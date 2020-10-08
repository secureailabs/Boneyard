classCenter <- function(x, label, prox, nNbr = min(table(label))-1) {
    ## nPrototype=rep(3, length(unique(label))), ...) {
    label <- as.character(label)
    clsLabel <- unique(label)
    ## Find the nearest nNbr neighbors of each case
    ## (including the case itself). 
    idx <- t(apply(prox, 1, order, decreasing=TRUE)[1:nNbr,])
    ## Find the class labels of the neighbors.
    cls <- label[idx]
    dim(cls) <- dim(idx)
    ## Count the number of neighbors in each class for each case.
    ncls <- sapply(clsLabel, function(x) rowSums(cls == x))
    ## For each class, find the case(s) with most neighbors in that class.
    clsMode <- max.col(t(ncls)) 
    ## Identify the neighbors of the class modes that are of the target class.
    nbrList <- mapply(function(cls, m) idx[m,][label[idx[m,]] == cls],
                      clsLabel, clsMode, SIMPLIFY=FALSE)
    ## Get the X data for the neighbors of the class `modes'.
    xdat <- t(sapply(nbrList, function(i) apply(x[i,,drop=FALSE], 2,
                                                  median)))
    xdat
}    
combine <- function(...) {
   pad0 <- function(x, len) c(x, rep(0, len-length(x)))
   padm0 <- function(x, len) rbind(x, matrix(0, nrow=len-nrow(x),
                                             ncol=ncol(x)))
   rflist <- list(...)
   areForest <- sapply(rflist, function(x) inherits(x, "randomForest")) 
   if (any(!areForest)) stop("Argument must be a list of randomForest objects")
   ## Use the first component as a template
   rf <- rflist[[1]]
   classRF <- rf$type == "classification"
   trees <- sapply(rflist, function(x) x$ntree)
   ntree <- sum(trees)
   rf$ntree <- ntree
   nforest <- length(rflist)
   haveTest <- ! any(sapply(rflist, function(x) is.null(x$test)))
   ## Check if predictor variables are identical.
   vlist <- lapply(rflist, function(x) rownames(importance(x)))
   numvars <- sapply(vlist, length)
   if (! all(numvars[1] == numvars[-1]))
       stop("Unequal number of predictor variables in the randomForest objects.")
   for (i in seq_along(vlist)) {
       if (! all(vlist[[i]] == vlist[[1]]))
           stop("Predictor variables are different in the randomForest objects.")
   }
   ## Combine the forest component, if any
   haveForest <- sapply(rflist, function(x) !is.null(x$forest))
   if (all(haveForest)) {
       nrnodes <- max(sapply(rflist, function(x) x$forest$nrnodes))
       rf$forest$nrnodes <- nrnodes
       rf$forest$ndbigtree <-
           unlist(sapply(rflist, function(x) x$forest$ndbigtree))
       rf$forest$nodestatus <-
           do.call("cbind", lapply(rflist, function(x)
                                   padm0(x$forest$nodestatus, nrnodes)))
       rf$forest $bestvar <-
           do.call("cbind",
                   lapply(rflist, function(x)
                          padm0(x$forest$bestvar, nrnodes)))
       rf$forest$xbestsplit <-
           do.call("cbind",
                   lapply(rflist, function(x)
                          padm0(x$forest$xbestsplit, nrnodes)))
       rf$forest$nodepred <-
           do.call("cbind", lapply(rflist, function(x)
                                   padm0(x$forest$nodepred, nrnodes)))
       tree.dim <- dim(rf$forest$treemap)
       if (classRF) {
           rf$forest$treemap <-
               array(unlist(lapply(rflist, function(x) apply(x$forest$treemap, 2:3,
                                                             pad0, nrnodes))),
                     c(nrnodes, 2, ntree))
       } else {
           rf$forest$leftDaughter <-
               do.call("cbind",
                       lapply(rflist, function(x)
                          padm0(x$forest$leftDaughter, nrnodes)))
           rf$forest$rightDaughter <-
               do.call("cbind",
                                  lapply(rflist, function(x)
                          padm0(x$forest$rightDaughter, nrnodes)))
       }
           rf$forest$ntree <- ntree
       if (classRF) rf$forest$cutoff <- rflist[[1]]$forest$cutoff
   } else {
       rf$forest <- NULL
   }
   
   if (classRF) {
       ## Combine the votes matrix: 
       rf$votes <- 0
       rf$oob.times <- 0
       areVotes <- all(sapply(rflist, function(x) any(x$votes > 1, na.rf=TRUE)))
       if (areVotes) {
           for(i in 1:nforest) {
               rf$oob.times <- rf$oob.times + rflist[[i]]$oob.times
               rf$votes <- rf$votes +
                   ifelse(is.na(rflist[[i]]$votes), 0, rflist[[i]]$votes)
           }
       } else {
           for(i in 1:nforest) {
               rf$oob.times <- rf$oob.times + rflist[[i]]$oob.times            
               rf$votes <- rf$votes +
                   ifelse(is.na(rflist[[i]]$votes), 0, rflist[[i]]$votes) *
                       rflist[[i]]$oob.times
           }
           rf$votes <- rf$votes / rf$oob.times
       }
       rf$predicted <- factor(colnames(rf$votes)[max.col(rf$votes)],
                              levels=levels(rf$predicted))
       if(haveTest) {
           rf$test$votes <- 0
           if (any(rf$test$votes > 1)) {
               for(i in 1:nforest)
                   rf$test$votes <- rf$test$votes + rflist[[i]]$test$votes
           } else {
               for (i in 1:nforest)
                   rf$test$votes <- rf$test$votes +
                       rflist[[i]]$test$votes * rflist[[i]]$ntree
           }
           rf$test$predicted <-
               factor(colnames(rf$test$votes)[max.col(rf$test$votes)],
                      levels=levels(rf$test$predicted))
       }
   } else {
       rf$predicted <- 0
       for (i in 1:nforest) rf$predicted <- rf$predicted +
           rflist[[i]]$predicted * rflist[[i]]$ntree
       rf$predicted <- rf$predicted / ntree
       if (haveTest) {
           rf$test$predicted <- 0
           for (i in 1:nforest) rf$test$predicted <- rf$test$predicted +
               rflist[[i]]$test$predicted * rflist[[i]]$ntree
           rf$test$predicted <- rf$test$predicted / ntree
       }
   }
   
   ## If variable importance is in all of them, compute the average
   ## (weighted by the number of trees in each forest)
   have.imp <- !any(sapply(rflist, function(x) is.null(x$importance)))
   if (have.imp) {
       rf$importance <- rf$importanceSD <- 0
       for(i in 1:nforest) {
           rf$importance <- rf$importance +
               rflist[[i]]$importance * rflist[[i]]$ntree
           ## Do the same thing with SD of importance, though that's not
           ## exactly right...
           rf$importanceSD <- rf$importanceSD +
               rflist[[i]]$importanceSD^2 * rflist[[i]]$ntree
       }
       rf$importance <- rf$importance / ntree
       rf$importanceSD <- sqrt(rf$importanceSD / ntree)
       haveCaseImp <- !any(sapply(rflist, function(x)
                                  is.null(x$localImportance)))
       ## Average casewise importance
       if (haveCaseImp) {
           rf$localImportance <- 0
           for (i in 1:nforest) {
               rf$localImportance <- rf$localImportance +
                   rflist[[i]]$localImportance * rflist[[i]]$ntree
           }
           rf$localImportance <- rf$localImportance / ntree
       }
   }
   
   ## If proximity is in all of them, compute the average
   ## (weighted by the number of trees in each forest)
   have.prox <- !any(sapply(rflist, function(x) is.null(x$proximity)))
   if (have.prox) {
       rf$proximity <- 0
       for(i in 1:nforest)
           rf$proximity <- rf$proximity + rflist[[i]]$proximity * rflist[[i]]$ntree
       rf$proximity <- rf$proximity / ntree
   }
   	
	## if there are inbag matrices, combine them as well.
	hasInBag <- all(sapply(rflist, function(x) !is.null(x$inbag)))
   	if (hasInBag) rf$inbag <- do.call(cbind, lapply(rflist, "[[", "inbag"))
   	## Set confusion matrix and error rates to NULL
   	if (classRF) {
       	rf$confusion <- NULL
       	rf$err.rate <- NULL
       	if (haveTest) {
           	rf$test$confusion <- NULL
           	rf$err.rate <- NULL
       	}
   	} else {
    	rf$mse <- rf$rsq <- NULL
       	if (haveTest) rf$test$mse <- rf$test$rsq <- NULL
   	}   
   	rf
}
getTree <- function(rfobj, k=1, labelVar=FALSE) {
  if (is.null(rfobj$forest)) {
    stop("No forest component in ", deparse(substitute(rfobj)))
  }
  if (k > rfobj$ntree) {
    stop("There are fewer than ", k, "trees in the forest")
  }
  if (rfobj$type == "regression") {
      tree <- cbind(rfobj$forest$leftDaughter[,k],
                    rfobj$forest$rightDaughter[,k],
                    rfobj$forest$bestvar[,k],
                    rfobj$forest$xbestsplit[,k],
                    rfobj$forest$nodestatus[,k],
                    rfobj$forest$nodepred[,k])[1:rfobj$forest$ndbigtree[k],]
  } else {
      tree <- cbind(rfobj$forest$treemap[,,k],
                    rfobj$forest$bestvar[,k],
                    rfobj$forest$xbestsplit[,k],
                    rfobj$forest$nodestatus[,k],
                    rfobj$forest$nodepred[,k])[1:rfobj$forest$ndbigtree[k],]
  }

  dimnames(tree) <- list(1:nrow(tree), c("left daughter", "right daughter",
                                         "split var", "split point",
                                         "status", "prediction"))

  if (labelVar) {
      tree <- as.data.frame(tree)
      v <- tree[[3]]
      v[v == 0] <- NA
      tree[[3]] <- factor(rownames(rfobj$importance)[v])
      if (rfobj$type == "classification") {
          v <- tree[[6]]
          v[! v %in% 1:nlevels(rfobj$y)] <- NA
          tree[[6]] <- levels(rfobj$y)[v]
      }
  }
  tree
}

grow <- function(x, ...) UseMethod("grow")

grow.default <- function(x, ...)
  stop("grow has not been implemented for this class of object")

grow.randomForest <- function(x, how.many, ...) {
  y <- update(x, ntree=how.many)
  combine(x, y)
}
importance <- function(x, ...)  UseMethod("importance")

importance.default <- function(x, ...)
    stop("No method implemented for this class of object")

importance.randomForest <- function(x, type=NULL, class=NULL, scale=TRUE,
                                    ...) {
    if (!inherits(x, "randomForest"))
        stop("x is not of class randomForest")
    classRF <- x$type != "regression"
    hasImp <- !is.null(dim(x$importance)) || ncol(x$importance) == 1
    hasType <- !is.null(type)
    if (hasType && type == 1 && !hasImp)
        stop("That measure has not been computed")
    allImp <- is.null(type) && hasImp
    if (hasType) {
        if (!(type %in% 1:2)) stop("Wrong type specified")
        if (type == 2 && !is.null(class))
            stop("No class-specific measure for that type")
    }
    
    imp <- x$importance
    if (hasType && type == 2) {
        if (hasImp) imp <- imp[, ncol(imp), drop=FALSE]
    } else {
        if (scale) {
            SD <- x$importanceSD
            imp[, -ncol(imp)] <-
                imp[, -ncol(imp), drop=FALSE] /
                    ifelse(SD < .Machine$double.eps, 1, SD)
        }
        if (!allImp) {
            if (is.null(class)) {
                ## The average decrease in accuracy measure:
                imp <- imp[, ncol(imp) - 1, drop=FALSE]
            } else {
                whichCol <- if (classRF) match(class, colnames(imp)) else 1
                if (is.na(whichCol)) stop(paste("Class", class, "not found."))
                imp <- imp[, whichCol, drop=FALSE]
            }
        }
    }
    imp
}
margin <- function(x, ...) {
    UseMethod("margin")
}

margin.randomForest <- function(x, ...) {
    if (x$type == "regression") {
        stop("margin not defined for regression Random Forests")
    }
    if( is.null(x$votes) ) {
        stop("margin is only defined if votes are present")
    }
    margin(x$votes, x$y, ...)
}

margin.default <- function(x, observed, ...) {
    if ( !is.factor(observed) ) {
        stop(deparse(substitute(observed)), " is not a factor")
    }
    if (ncol(x) != nlevels(observed))
        stop("number of columns in x must equal the number of levels in observed")
    if (! all(colnames(x) %in% levels(observed)) ||
        ! all(levels(observed) %in% colnames(x)))
        stop("column names of x must match levels of observed")
    ## If the votes are not in fractions, normalize them to fractions.
    if ( any(x > 1) ) x <- sweep(x, 1, rowSums(x), "/")
    position <- match(as.character(observed), colnames(x))
    margin <- numeric(length(observed))
    for (i in seq_along(observed)) {
        margin[i] <- x[i, position[i]] - max(x[i, -position[i]])
    }
    names(margin) <- observed
    class(margin) <- "margin"
    margin
}

plot.margin <- function(x, sort=TRUE, ...) {
    if (sort) x <- sort(x)
    nF <- factor(names(x))
    nlevs <- length(levels(nF))
    if ( requireNamespace("RColorBrewer", quietly=TRUE) && nlevs < 12) {
        pal <- RColorBrewer::brewer.pal(nlevs,"Set1")
    } else {
        pal <- rainbow(nlevs)
    }
    plot.default(x, col=pal[as.numeric(nF)], pch=20, ... )
}

na.roughfix <- function(object, ...)
  UseMethod("na.roughfix")

na.roughfix.data.frame <- function(object, ...) {
  isfac <- sapply(object, is.factor)
  isnum <- sapply(object, is.numeric)
  if (any(!(isfac | isnum)))
      stop("na.roughfix only works for numeric or factor")
  roughfix <- function(x) {
      if (any(is.na(x))) {
          if (is.factor(x)) {
              freq <- table(x)
              x[is.na(x)] <- names(freq)[which.max(freq)]
          } else {
              x[is.na(x)] <- median(x, na.rm=TRUE)
          }
      }
      x
  }
  object[] <- lapply(object, roughfix)
  object
}

na.roughfix.default <- function(object, ...) {
  if (!is.atomic(object))
    return(object)
  d <- dim(object)
  if (length(d) > 2)
    stop("can't handle objects with more than two dimensions")
  if (all(!is.na(object)))
    return(object)
  if (!is.numeric(object))
    stop("roughfix can only deal with numeric data.")
  if (length(d) == 2) {
      hasNA <- which(apply(object, 2, function(x) any(is.na(x))))
      for (j in hasNA)
          object[is.na(object[, j]), j] <- median(object[, j], na.rm=TRUE)
  } else {
      object[is.na(object)] <- median(object, na.rm=TRUE)
  }
  object
}
outlier <- function(x, ...) UseMethod("outlier")

outlier.randomForest <- function(x, ...) {
    if (!inherits(x, "randomForest")) stop("x is not a randomForest object")
    if (x$type == "regression") stop("no outlier measure for regression")
    if (is.null(x$proximity)) stop("no proximity measures available")
    outlier.default(x$proximity, x$y)
}

outlier.default <- function(x, cls=NULL, ...) {
    if (nrow(x) != ncol(x)) stop ("x must be a square matrix")
    n <- nrow(x)
    if (is.null(cls)) cls <- rep(1, n)
    cls <- factor(cls)
    lvl <- levels(cls)
    cls.n <- table(cls)[lvl]
    id <- if (is.null(rownames(x))) 1:n else rownames(x)
    outlier <- structure(rep(NA, n), names=id)
    for (i in lvl) {
        iclass <- cls == i
        out <- rowSums(x[iclass, iclass]^2)
        out <- n / ifelse(out == 0, 1, out)
        out <- (out - median(out)) / mad(out)
        outlier[iclass] <- out
    }
    outlier
}
"predict.randomForest" <-
    function (object, newdata, type = "response", norm.votes = TRUE,
              predict.all=FALSE, proximity = FALSE, nodes=FALSE, cutoff, ...)
{
    if (!inherits(object, "randomForest"))
        stop("object not of class randomForest")
    if (is.null(object$forest)) stop("No forest component in the object")
    out.type <- charmatch(tolower(type),
                          c("response", "prob", "vote", "class"))
    if (is.na(out.type))
        stop("type must be one of 'response', 'prob', 'vote'")
    if (out.type == 4) out.type <- 1
    if (out.type != 1 && object$type == "regression")
        stop("'prob' or 'vote' not meaningful for regression")
    if (out.type == 2)
        norm.votes <- TRUE
    if (missing(newdata)) {
		p <- if (! is.null(object$na.action)) {
			napredict(object$na.action, object$predicted)
		} else {
			object$predicted
		}
        if (object$type == "regression") return(p)
        if (proximity & is.null(object$proximity))
            warning("cannot return proximity without new data if random forest object does not already have proximity")
        if (out.type == 1) {
            if (proximity) {
                return(list(pred = p,
                            proximity = object$proximity))
            } else return(p)
        }
		v <- object$votes
		if (!is.null(object$na.action)) v <- napredict(object$na.action, v)
        if (norm.votes) {
            t1 <- t(apply(v, 1, function(x) { x/sum(x) }))
            class(t1) <- c(class(t1), "votes")
            if (proximity) return(list(pred = t1, proximity = object$proximity))
            else return(t1)
        } else {
            if (proximity) return(list(pred = v, proximity = object$proximity))
            else return(v)
        }
    }
    if (missing(cutoff)) {
        cutoff <- object$forest$cutoff
    } else {
        if (sum(cutoff) > 1 || sum(cutoff) < 0 || !all(cutoff > 0) ||
            length(cutoff) != length(object$classes)) {
            stop("Incorrect cutoff specified.")
        }
        if (!is.null(names(cutoff))) {
            if (!all(names(cutoff) %in% object$classes)) {
                stop("Wrong name(s) for cutoff")
            }
            cutoff <- cutoff[object$classes]
        }
    }

    if (object$type == "unsupervised")
        stop("Can't predict unsupervised forest.")

    if (inherits(object, "randomForest.formula")) {
        newdata <- as.data.frame(newdata)
        rn <- row.names(newdata)
        Terms <- delete.response(object$terms)
        x <- model.frame(Terms, newdata, na.action = na.omit)
        keep <- match(row.names(x), rn)
    } else {
        if (is.null(dim(newdata)))
            dim(newdata) <- c(1, length(newdata))
        x <- newdata
        if (nrow(x) == 0)
            stop("newdata has 0 rows")
        if (any(is.na(x)))
            stop("missing values in newdata")
        keep <- 1:nrow(x)
        rn <- rownames(x)
        if (is.null(rn)) rn <- keep
    }
    vname <- if (is.null(dim(object$importance))) {
        names(object$importance)
    } else {
        rownames(object$importance)
    }
    if (is.null(colnames(x))) {
        if (ncol(x) != length(vname)) {
            stop("number of variables in newdata does not match that in the training data")
        }
    } else {
        if (any(! vname %in% colnames(x)))
            stop("variables in the training data missing in newdata")
        x <- x[, vname, drop=FALSE]
    }
    if (is.data.frame(x)) {
		isFactor <- function(x) is.factor(x) & ! is.ordered(x)
        xfactor <- which(sapply(x, isFactor))
        if (length(xfactor) > 0 && "xlevels" %in% names(object$forest)) {
            for (i in xfactor) {
                if (any(! levels(x[[i]]) %in% object$forest$xlevels[[i]]))
                    stop("New factor levels not present in the training data")
                x[[i]] <-
                    factor(x[[i]],
                           levels=levels(x[[i]])[match(levels(x[[i]]), object$forest$xlevels[[i]])])
            }
        }
        cat.new <- sapply(x, function(x) if (is.factor(x) && !is.ordered(x))
                          length(levels(x)) else 1)
        if (!all(object$forest$ncat == cat.new))
            stop("Type of predictors in new data do not match that of the training data.")
    }
    mdim <- ncol(x)
    ntest <- nrow(x)
    ntree <- object$forest$ntree
    maxcat <- max(object$forest$ncat)
    nclass <- object$forest$nclass
    nrnodes <- object$forest$nrnodes
    ## get rid of warning:
    op <- options(warn=-1)
    on.exit(options(op))
    x <- t(data.matrix(x))

    if (predict.all) {
        treepred <- if (object$type == "regression") {
            matrix(double(ntest * ntree), ncol=ntree)
        } else {
            matrix(integer(ntest * ntree), ncol=ntree)
        }
    } else {
        treepred <- numeric(ntest)
    }
    proxmatrix <- if (proximity) matrix(0, ntest, ntest) else numeric(1)
    nodexts <- if (nodes) integer(ntest * ntree) else integer(ntest)

    if (object$type == "regression") {
            if (!is.null(object$forest$treemap)) {
                object$forest$leftDaughter <-
                    object$forest$treemap[,1,, drop=FALSE]
                object$forest$rightDaughter <-
                    object$forest$treemap[,2,, drop=FALSE]
                object$forest$treemap <- NULL
            }

            keepIndex <- "ypred"
            if (predict.all) keepIndex <- c(keepIndex, "treepred")
            if (proximity) keepIndex <- c(keepIndex, "proximity")
            if (nodes) keepIndex <- c(keepIndex, "nodexts")
            ## Ensure storage mode is what is expected in C.
            if (! is.integer(object$forest$leftDaughter))
                storage.mode(object$forest$leftDaughter) <- "integer"
            if (! is.integer(object$forest$rightDaughter))
                storage.mode(object$forest$rightDaughter) <- "integer"
            if (! is.integer(object$forest$nodestatus))
                storage.mode(object$forest$nodestatus) <- "integer"
            if (! is.double(object$forest$xbestsplit))
                storage.mode(object$forest$xbestsplit) <- "double"
            if (! is.double(object$forest$nodepred))
                storage.mode(object$forest$nodepred) <- "double"
            if (! is.integer(object$forest$bestvar))
                storage.mode(object$forest$bestvar) <- "integer"
            if (! is.integer(object$forest$ndbigtree))
                storage.mode(object$forest$ndbigtree) <- "integer"
            if (! is.integer(object$forest$ncat))
                storage.mode(object$forest$ncat) <- "integer"

            ans <- .C("regForest",
                  as.double(x),
                  ypred = double(ntest),
                  as.integer(mdim),
                  as.integer(ntest),
                  as.integer(ntree),
                  object$forest$leftDaughter,
                  object$forest$rightDaughter,
                  object$forest$nodestatus,
                  nrnodes,
                  object$forest$xbestsplit,
                  object$forest$nodepred,
                  object$forest$bestvar,
                  object$forest$ndbigtree,
                  object$forest$ncat,
                  as.integer(maxcat),
                  as.integer(predict.all),
                  treepred = as.double(treepred),
                  as.integer(proximity),
                  proximity = as.double(proxmatrix),
                  nodes = as.integer(nodes),
                  nodexts = as.integer(nodexts),
                  DUP=FALSE,
                  PACKAGE = "randomForest")[keepIndex]
            ## Apply bias correction if needed.
            yhat <- rep(NA, length(rn))
            names(yhat) <- rn
            if (!is.null(object$coefs)) {
                yhat[keep] <- object$coefs[1] + object$coefs[2] * ans$ypred
            } else {
                yhat[keep] <- ans$ypred
            }
            if (predict.all) {
                treepred <- matrix(NA, length(rn), ntree,
                                   dimnames=list(rn, NULL))
                treepred[keep,] <- ans$treepred
            }
            if (!proximity) {
                res <- if (predict.all)
                    list(aggregate=yhat, individual=treepred) else yhat
            } else {
                res <- list(predicted = yhat,
                            proximity = structure(ans$proximity,
                            dim=c(ntest, ntest), dimnames=list(rn, rn)))
            }
            if (nodes) {
                attr(res, "nodes") <- matrix(ans$nodexts, ntest, ntree,
                                             dimnames=list(rn[keep], 1:ntree))
            }
        } else {
        countts <- matrix(0, ntest, nclass)
        t1 <- .C("classForest",
                 mdim = as.integer(mdim),
                 ntest = as.integer(ntest),
                 nclass = as.integer(object$forest$nclass),
                 maxcat = as.integer(maxcat),
                 nrnodes = as.integer(nrnodes),
                 jbt = as.integer(ntree),
                 xts = as.double(x),
                 xbestsplit = as.double(object$forest$xbestsplit),
                 pid = object$forest$pid,
                 cutoff = as.double(cutoff),
                 countts = as.double(countts),
                 treemap = as.integer(aperm(object$forest$treemap,
                                 c(2, 1, 3))),
                 nodestatus = as.integer(object$forest$nodestatus),
                 cat = as.integer(object$forest$ncat),
                 nodepred = as.integer(object$forest$nodepred),
                 treepred = as.integer(treepred),
                 jet = as.integer(numeric(ntest)),
                 bestvar = as.integer(object$forest$bestvar),
                 nodexts = as.integer(nodexts),
                 ndbigtree = as.integer(object$forest$ndbigtree),
                 predict.all = as.integer(predict.all),
                 prox = as.integer(proximity),
                 proxmatrix = as.double(proxmatrix),
                 nodes = as.integer(nodes),
                 DUP=FALSE,
                 PACKAGE = "randomForest")
        if (out.type > 1) {
            out.class.votes <- t(matrix(t1$countts, nrow = nclass, ncol = ntest))
            if (norm.votes)
                out.class.votes <-
                    sweep(out.class.votes, 1, rowSums(out.class.votes), "/")
            z <- matrix(NA, length(rn), nclass,
                        dimnames=list(rn, object$classes))
            z[keep, ] <- out.class.votes
             class(z) <- c(class(z), "votes")
            res <- z
        } else {
            out.class <- factor(rep(NA, length(rn)),
                                levels=1:length(object$classes),
                                labels=object$classes)
            out.class[keep] <- object$classes[t1$jet]
            names(out.class)[keep] <- rn[keep]
            res <- out.class
        }
        if (predict.all) {
            treepred <- matrix(object$classes[t1$treepred],
                               nrow=length(keep), dimnames=list(rn[keep], NULL))
            res <- list(aggregate=res, individual=treepred)
        }
        if (proximity)
            res <- list(predicted = res, proximity = structure(t1$proxmatrix,
                                         dim = c(ntest, ntest),
                                         dimnames = list(rn[keep], rn[keep])))
        if (nodes) attr(res, "nodes") <- matrix(t1$nodexts, ntest, ntree,
                                                dimnames=list(rn[keep], 1:ntree))
    }
    res
}
"print.randomForest" <-
function(x, ...) {
  cat("\nCall:\n", deparse(x$call), "\n")
  cat("               Type of random forest: ", x$type, "\n", sep="")
  cat("                     Number of trees: ", x$ntree, "\n",sep="")
  cat("No. of variables tried at each split: ", x$mtry, "\n\n", sep="")
  if(x$type == "classification") {
    if(!is.null(x$confusion)) {
      cat("        OOB estimate of  error rate: ",
          round(x$err.rate[x$ntree, "OOB"]*100, digits=2), "%\n", sep="")
      cat("Confusion matrix:\n")
      print(x$confusion)
      if(!is.null(x$test$err.rate)) {
        cat("                Test set error rate: ",
            round(x$test$err.rate[x$ntree, "Test"]*100, digits=2), "%\n",
            sep="")
        cat("Confusion matrix:\n")
        print(x$test$confusion)
      }
    }
  }
  if(x$type == "regression") {
    if(!is.null(x$mse)) {
      cat("          Mean of squared residuals: ", x$mse[length(x$mse)],
          "\n", sep="")
      cat("                    % Var explained: ",
          round(100*x$rsq[length(x$rsq)], digits=2), "\n", sep="")
      if(!is.null(x$test$mse)) {
        cat("                       Test set MSE: ",
            round(x$test$mse[length(x$test$mse)], digits=2), "\n", sep="")
        cat("                    % Var explained: ",
            round(100*x$test$rsq[length(x$test$rsq)], digits=2), "\n", sep="")
      }      
    }
    if (!is.null(x$coefs)) {
      cat("  Bias correction applied:\n")
      cat("  Intercept: ", x$coefs[1], "\n")
      cat("      Slope: ", x$coefs[2], "\n")
    }
  }
}
## mylevels() returns levels if given a factor, otherwise 0.
mylevels <- function(x) if (is.factor(x)) levels(x) else 0

"randomForest.default" <-
    function(x, y=NULL,  xtest=NULL, ytest=NULL, ntree=500,
             mtry=if (!is.null(y) && !is.factor(y))
             max(floor(ncol(x)/3), 1) else floor(sqrt(ncol(x))),
             replace=TRUE, classwt=NULL, cutoff, strata,
             sampsize = if (replace) nrow(x) else ceiling(.632*nrow(x)),
             nodesize = if (!is.null(y) && !is.factor(y)) 5 else 1,
             maxnodes=NULL,
             importance=FALSE, localImp=FALSE, nPerm=1,
             proximity, oob.prox=proximity,
             norm.votes=TRUE, do.trace=FALSE,
             keep.forest=!is.null(y) && is.null(xtest), corr.bias=FALSE,
             keep.inbag=FALSE, ...) {
    addclass <- is.null(y)
    classRF <- addclass || is.factor(y)
    if (!classRF && length(unique(y)) <= 5) {
        warning("The response has five or fewer unique values.  Are you sure you want to do regression?")
    }
    if (classRF && !addclass && length(unique(y)) < 2)
        stop("Need at least two classes to do classification.")
    n <- nrow(x)
    p <- ncol(x)
    if (n == 0) stop("data (x) has 0 rows")
    x.row.names <- rownames(x)
    x.col.names <- if (is.null(colnames(x))) 1:ncol(x) else colnames(x)

    ## overcome R's lazy evaluation:
    keep.forest <- keep.forest

    testdat <- !is.null(xtest)
    if (testdat) {
        if (ncol(x) != ncol(xtest))
            stop("x and xtest must have same number of columns")
        ntest <- nrow(xtest)
        xts.row.names <- rownames(xtest)
    }

    ## Make sure mtry is in reasonable range.
    if (mtry < 1 || mtry > p)
        warning("invalid mtry: reset to within valid range")
    mtry <- max(1, min(p, round(mtry)))
    if (!is.null(y)) {
        if (length(y) != n) stop("length of response must be the same as predictors")
        addclass <- FALSE
    } else {
        if (!addclass) addclass <- TRUE
        y <- factor(c(rep(1, n), rep(2, n)))
        x <- rbind(x, x)
    }

    ## Check for NAs.
    if (any(is.na(x))) stop("NA not permitted in predictors")
    if (testdat && any(is.na(xtest))) stop("NA not permitted in xtest")
    if (any(is.na(y))) stop("NA not permitted in response")
    if (!is.null(ytest) && any(is.na(ytest))) stop("NA not permitted in ytest")

    if (is.data.frame(x)) {
        xlevels <- lapply(x, mylevels)
        ncat <- sapply(xlevels, length)
        ## Treat ordered factors as numerics.
        ncat <- ifelse(sapply(x, is.ordered), 1, ncat)
        x <- data.matrix(x)
        if(testdat) {
            if(!is.data.frame(xtest))
                stop("xtest must be data frame if x is")
            xfactor <- which(sapply(xtest, is.factor))
            if (length(xfactor) > 0) {
                for (i in xfactor) {
                    if (any(! levels(xtest[[i]]) %in% xlevels[[i]]))
                        stop("New factor levels in xtest not present in x")
                    xtest[[i]] <-
                        factor(xlevels[[i]][match(xtest[[i]], xlevels[[i]])],
                               levels=xlevels[[i]])
                }
            }
            xtest <- data.matrix(xtest)
        }
    } else {
        ncat <- rep(1, p)
		names(ncat) <- colnames(x)
        xlevels <- as.list(rep(0, p))
    }
    maxcat <- max(ncat)
    if (maxcat > 53)
        stop("Can not handle categorical predictors with more than 53 categories.")

    if (classRF) {
        nclass <- length(levels(y))
        ## Check for empty classes:
        if (any(table(y) == 0)) stop("Can't have empty classes in y.")
        if (!is.null(ytest)) {
            if (!is.factor(ytest)) stop("ytest must be a factor")
            if (!all(levels(y) == levels(ytest)))
                stop("y and ytest must have the same levels")
        }
        if (missing(cutoff)) {
            cutoff <- rep(1 / nclass, nclass)
        } else {
            if (sum(cutoff) > 1 || sum(cutoff) < 0 || !all(cutoff > 0) ||
                length(cutoff) != nclass) {
                stop("Incorrect cutoff specified.")
            }
            if (!is.null(names(cutoff))) {
                if (!all(names(cutoff) %in% levels(y))) {
                    stop("Wrong name(s) for cutoff")
                }
                cutoff <- cutoff[levels(y)]
            }
        }
        if (!is.null(classwt)) {
            if (length(classwt) != nclass)
                stop("length of classwt not equal to number of classes")
            ## If classwt has names, match to class labels.
            if (!is.null(names(classwt))) {
                if (!all(names(classwt) %in% levels(y))) {
                    stop("Wrong name(s) for classwt")
                }
                classwt <- classwt[levels(y)]
            }
            if (any(classwt <= 0)) stop("classwt must be positive")
            ipi <- 1
        } else {
            classwt <- rep(1, nclass)
            ipi <- 0
        }
    } else addclass <- FALSE

    if (missing(proximity)) proximity <- addclass
    if (proximity) {
        prox <- matrix(0.0, n, n)
        proxts <- if (testdat) matrix(0, ntest, ntest + n) else double(1)
    } else {
        prox <- proxts <- double(1)
    }

    if (localImp) {
        importance <- TRUE
        impmat <- matrix(0, p, n)
    } else impmat <- double(1)

    if (importance) {
        if (nPerm < 1) nPerm <- as.integer(1) else nPerm <- as.integer(nPerm)
        if (classRF) {
            impout <- matrix(0.0, p, nclass + 2)
            impSD <- matrix(0.0, p, nclass + 1)
        } else {
            impout <- matrix(0.0, p, 2)
            impSD <- double(p)
            names(impSD) <- x.col.names
        }
    } else {
        impout <- double(p)
        impSD <- double(1)
    }

    nsample <- if (addclass) 2 * n else n
    Stratify <- length(sampsize) > 1
    if ((!Stratify) && sampsize > nrow(x)) stop("sampsize too large")
    if (Stratify && (!classRF)) stop("sampsize should be of length one")
    if (classRF) {
        if (Stratify) {
            if (missing(strata)) strata <- y
            if (!is.factor(strata)) strata <- as.factor(strata)
            nsum <- sum(sampsize)
            if (length(sampsize) > nlevels(strata))
                stop("sampsize has too many elements.")
            if (any(sampsize <= 0) || nsum == 0)
                stop("Bad sampsize specification")
            ## If sampsize has names, match to class labels.
            if (!is.null(names(sampsize))) {
                sampsize <- sampsize[levels(strata)]
            }
            if (any(sampsize > table(strata)))
              stop("sampsize can not be larger than class frequency")
        } else {
            nsum <- sampsize
        }
        nrnodes <- 2 * trunc(nsum / nodesize) + 1
    } else {
        ## For regression trees, need to do this to get maximal trees.
        nrnodes <- 2 * trunc(sampsize/max(1, nodesize - 4)) + 1
    }
    if (!is.null(maxnodes)) {
        ## convert # of terminal nodes to total # of nodes
        maxnodes <- 2 * maxnodes - 1
        if (maxnodes > nrnodes) warning("maxnodes exceeds its max value.")
        nrnodes <- min(c(nrnodes, max(c(maxnodes, 1))))
    }
    ## Compiled code expects variables in rows and observations in columns.
    x <- t(x)
    storage.mode(x) <- "double"
    if (testdat) {
        xtest <- t(xtest)
        storage.mode(xtest) <- "double"
        if (is.null(ytest)) {
            ytest <- labelts <- 0
        } else {
            labelts <- TRUE
        }
    } else {
        xtest <- double(1)
        ytest <- double(1)
        ntest <- 1
        labelts <- FALSE
    }
    nt <- if (keep.forest) ntree else 1

    if (classRF) {
        cwt <- classwt
        threshold <- cutoff
        error.test <- if (labelts) double((nclass+1) * ntree) else double(1)
        rfout <- .C("classRF",
                    x = x,
                    xdim = as.integer(c(p, n)),
                    y = as.integer(y),
                    nclass = as.integer(nclass),
                    ncat = as.integer(ncat),
                    maxcat = as.integer(maxcat),
                    sampsize = as.integer(sampsize),
                    strata = if (Stratify) as.integer(strata) else integer(1),
                    Options = as.integer(c(addclass,
                    importance,
                    localImp,
                    proximity,
                    oob.prox,
                    do.trace,
                    keep.forest,
                    replace,
                    Stratify,
                    keep.inbag)),
                    ntree = as.integer(ntree),
                    mtry = as.integer(mtry),
                    ipi = as.integer(ipi),
                    classwt = as.double(cwt),
                    cutoff = as.double(threshold),
                    nodesize = as.integer(nodesize),
                    outcl = integer(nsample),
                    counttr = integer(nclass * nsample),
                    prox = prox,
                    impout = impout,
                    impSD = impSD,
                    impmat = impmat,
                    nrnodes = as.integer(nrnodes),
                    ndbigtree = integer(ntree),
                    nodestatus = integer(nt * nrnodes),
                    bestvar = integer(nt * nrnodes),
                    treemap = integer(nt * 2 * nrnodes),
                    nodepred = integer(nt * nrnodes),
                    xbestsplit = double(nt * nrnodes),
                    errtr = double((nclass+1) * ntree),
                    testdat = as.integer(testdat),
                    xts = as.double(xtest),
                    clts = as.integer(ytest),
                    nts = as.integer(ntest),
                    countts = double(nclass * ntest),
                    outclts = as.integer(numeric(ntest)),
                    labelts = as.integer(labelts),
                    proxts = proxts,
                    errts = error.test,
                    inbag = if (keep.inbag)
                    matrix(integer(n * ntree), n) else integer(n),
                    DUP=FALSE,
                    PACKAGE="randomForest")[-1]
        if (keep.forest) {
            ## deal with the random forest outputs
            max.nodes <- max(rfout$ndbigtree)
            treemap <- aperm(array(rfout$treemap, dim = c(2, nrnodes, ntree)),
                             c(2, 1, 3))[1:max.nodes, , , drop=FALSE]
        }
        if (!addclass) {
            ## Turn the predicted class into a factor like y.
            out.class <- factor(rfout$outcl, levels=1:nclass,
                                labels=levels(y))
            names(out.class) <- x.row.names
            con <- table(observed = y,
                         predicted = out.class)[levels(y), levels(y)]
            con <- cbind(con, class.error = 1 - diag(con)/rowSums(con))
        }
        out.votes <- t(matrix(rfout$counttr, nclass, nsample))[1:n, ]
        oob.times <- rowSums(out.votes)
        if (norm.votes)
            out.votes <- t(apply(out.votes, 1, function(x) x/sum(x)))
        dimnames(out.votes) <- list(x.row.names, levels(y))
        class(out.votes) <- c(class(out.votes), "votes")
        if (testdat) {
            out.class.ts <- factor(rfout$outclts, levels=1:nclass,
                                   labels=levels(y))
            names(out.class.ts) <- xts.row.names
            out.votes.ts <- t(matrix(rfout$countts, nclass, ntest))
            dimnames(out.votes.ts) <- list(xts.row.names, levels(y))
            if (norm.votes)
                out.votes.ts <- t(apply(out.votes.ts, 1,
                                        function(x) x/sum(x)))
            class(out.votes.ts) <- c(class(out.votes.ts), "votes")
            if (labelts) {
                testcon <- table(observed = ytest,
                                 predicted = out.class.ts)[levels(y), levels(y)]
                testcon <- cbind(testcon,
                                 class.error = 1 - diag(testcon)/rowSums(testcon))
            }
        }
        cl <- match.call()
        cl[[1]] <- as.name("randomForest")
        out <- list(call = cl,
                    type = if (addclass) "unsupervised" else "classification",
                    predicted = if (addclass) NULL else out.class,
                    err.rate = if (addclass) NULL else t(matrix(rfout$errtr,
                    nclass+1, ntree,
                    dimnames=list(c("OOB", levels(y)), NULL))),
                    confusion = if (addclass) NULL else con,
                    votes = out.votes,
                    oob.times = oob.times,
                    classes = levels(y),
                    importance = if (importance)
                    matrix(rfout$impout, p, nclass+2,
                           dimnames = list(x.col.names,
                           c(levels(y), "MeanDecreaseAccuracy",
                             "MeanDecreaseGini")))
                    else matrix(rfout$impout, ncol=1,
                                dimnames=list(x.col.names, "MeanDecreaseGini")),
                    importanceSD = if (importance)
                    matrix(rfout$impSD, p, nclass + 1,
                           dimnames = list(x.col.names,
                           c(levels(y), "MeanDecreaseAccuracy")))
                    else NULL,
                    localImportance = if (localImp)
                    matrix(rfout$impmat, p, n,
                           dimnames = list(x.col.names,x.row.names)) else NULL,
                    proximity = if (proximity) matrix(rfout$prox, n, n,
                    dimnames = list(x.row.names, x.row.names)) else NULL,
                    ntree = ntree,
                    mtry = mtry,
                    forest = if (!keep.forest) NULL else {
                        list(ndbigtree = rfout$ndbigtree,
                             nodestatus = matrix(rfout$nodestatus,
                             ncol = ntree)[1:max.nodes,, drop=FALSE],
                             bestvar = matrix(rfout$bestvar, ncol = ntree)[1:max.nodes,, drop=FALSE],
                             treemap = treemap,
                             nodepred = matrix(rfout$nodepred,
                             ncol = ntree)[1:max.nodes,, drop=FALSE],
                             xbestsplit = matrix(rfout$xbestsplit,
                             ncol = ntree)[1:max.nodes,, drop=FALSE],
                             pid = rfout$classwt, cutoff=cutoff, ncat=ncat,
                             maxcat = maxcat,
                             nrnodes = max.nodes, ntree = ntree,
                             nclass = nclass, xlevels=xlevels)
                    },
                    y = if (addclass) NULL else y,
                    test = if(!testdat) NULL else list(
                    predicted = out.class.ts,
                    err.rate = if (labelts) t(matrix(rfout$errts, nclass+1,
                    ntree,
                dimnames=list(c("Test", levels(y)), NULL))) else NULL,
                    confusion = if (labelts) testcon else NULL,
                    votes = out.votes.ts,
                    proximity = if(proximity) matrix(rfout$proxts, nrow=ntest,
                    dimnames = list(xts.row.names, c(xts.row.names,
                    x.row.names))) else NULL),
                    inbag = if (keep.inbag) matrix(rfout$inbag, nrow=nrow(rfout$inbag), 
										dimnames=list(x.row.names, NULL)) else NULL)
    } else {
		ymean <- mean(y)
		y <- y - ymean
		ytest <- ytest - ymean
        rfout <- .C("regRF",
                    x,
                    as.double(y),
                    as.integer(c(n, p)),
                    as.integer(sampsize),
                    as.integer(nodesize),
                    as.integer(nrnodes),
                    as.integer(ntree),
                    as.integer(mtry),
                    as.integer(c(importance, localImp, nPerm)),
                    as.integer(ncat),
                    as.integer(maxcat),
                    as.integer(do.trace),
                    as.integer(proximity),
                    as.integer(oob.prox),
                    as.integer(corr.bias),
                    ypred = double(n),
                    impout = impout,
                    impmat = impmat,
                    impSD = impSD,
                    prox = prox,
                    ndbigtree = integer(ntree),
                    nodestatus = matrix(integer(nrnodes * nt), ncol=nt),
                    leftDaughter = matrix(integer(nrnodes * nt), ncol=nt),
                    rightDaughter = matrix(integer(nrnodes * nt), ncol=nt),
                    nodepred = matrix(double(nrnodes * nt), ncol=nt),
                    bestvar = matrix(integer(nrnodes * nt), ncol=nt),
                    xbestsplit = matrix(double(nrnodes * nt), ncol=nt),
                    mse = double(ntree),
                    keep = as.integer(c(keep.forest, keep.inbag)),
                    replace = as.integer(replace),
                    testdat = as.integer(testdat),
                    xts = xtest,
                    ntest = as.integer(ntest),
                    yts = as.double(ytest),
                    labelts = as.integer(labelts),
                    ytestpred = double(ntest),
                    proxts = proxts,
                    msets = double(if (labelts) ntree else 1),
                    coef = double(2),
                    oob.times = integer(n),
                    inbag = if (keep.inbag)
                    matrix(integer(n * ntree), n) else integer(1),
                    DUP=FALSE,
                    PACKAGE="randomForest")[c(16:28, 36:41)]
        ## Format the forest component, if present.
        if (keep.forest) {
            max.nodes <- max(rfout$ndbigtree)
            rfout$nodestatus <-
                rfout$nodestatus[1:max.nodes, , drop=FALSE]
            rfout$bestvar <-
                rfout$bestvar[1:max.nodes, , drop=FALSE]
            rfout$nodepred <-
                rfout$nodepred[1:max.nodes, , drop=FALSE] + ymean
            rfout$xbestsplit <-
                rfout$xbestsplit[1:max.nodes, , drop=FALSE]
            rfout$leftDaughter <-
                rfout$leftDaughter[1:max.nodes, , drop=FALSE]
            rfout$rightDaughter <-
                rfout$rightDaughter[1:max.nodes, , drop=FALSE]
        }
        cl <- match.call()
        cl[[1]] <- as.name("randomForest")
        ## Make sure those obs. that have not been OOB get NA as prediction.
        ypred <- rfout$ypred
        if (any(rfout$oob.times < 1)) {
            ypred[rfout$oob.times == 0] <- NA
        }
        out <- list(call = cl,
                    type = "regression",
                    predicted = structure(ypred + ymean, names=x.row.names),
                    mse = rfout$mse,
                    rsq = 1 - rfout$mse / (var(y) * (n-1) / n),
                    oob.times = rfout$oob.times,
                    importance = if (importance) matrix(rfout$impout, p, 2,
                    dimnames=list(x.col.names,
                                  c("%IncMSE","IncNodePurity"))) else
                        matrix(rfout$impout, ncol=1,
                               dimnames=list(x.col.names, "IncNodePurity")),
                    importanceSD=if (importance) rfout$impSD else NULL,
                    localImportance = if (localImp)
                    matrix(rfout$impmat, p, n, dimnames=list(x.col.names,
                                               x.row.names)) else NULL,
                    proximity = if (proximity) matrix(rfout$prox, n, n,
                    dimnames = list(x.row.names, x.row.names)) else NULL,
                   ntree = ntree,
                    mtry = mtry,
                    forest = if (keep.forest)
                    c(rfout[c("ndbigtree", "nodestatus", "leftDaughter",
                              "rightDaughter", "nodepred", "bestvar",
                              "xbestsplit")],
                      list(ncat = ncat), list(nrnodes=max.nodes),
                      list(ntree=ntree), list(xlevels=xlevels)) else NULL,
                    coefs = if (corr.bias) rfout$coef else NULL,
                    y = y + ymean,
                    test = if(testdat) {
                        list(predicted = structure(rfout$ytestpred + ymean,
                             names=xts.row.names),
                             mse = if(labelts) rfout$msets else NULL,
                             rsq = if(labelts) 1 - rfout$msets /
                                        (var(ytest) * (n-1) / n) else NULL,
                             proximity = if (proximity)
                             matrix(rfout$proxts / ntree, nrow = ntest,
                                    dimnames = list(xts.row.names,
                                    c(xts.row.names,
                                    x.row.names))) else NULL)
                    } else NULL,
                    inbag = if (keep.inbag)
                    matrix(rfout$inbag, nrow(rfout$inbag),
                           dimnames=list(x.row.names, NULL)) else NULL)
    }
    class(out) <- "randomForest"
    return(out)
}
"randomForest.formula" <-
    function(formula, data = NULL, ..., subset, na.action = na.fail) {
### formula interface for randomForest.
### code gratefully stolen from svm.formula (package e1071).
###
    if (!inherits(formula, "formula"))
        stop("method is only for formula objects")
    m <- match.call(expand.dots = FALSE)
    ## Catch xtest and ytest in arguments.
    if (any(c("xtest", "ytest") %in% names(m)))
        stop("xtest/ytest not supported through the formula interface")
    names(m)[2] <- "formula"
    if (is.matrix(eval(m$data, parent.frame())))
        m$data <- as.data.frame(data)
    m$... <- NULL
    m$na.action <- na.action
    m[[1]] <- as.name("model.frame")
    m <- eval(m, parent.frame())
	#rn <- 1:nrow(m)
	
    y <- model.response(m)
    Terms <- attr(m, "terms")
    attr(Terms, "intercept") <- 0
	attr(y, "na.action") <- attr(m, "na.action")
	## Drop any "negative" terms in the formula.
    ## test with:
    ## randomForest(Fertility~.-Catholic+I(Catholic<50),data=swiss,mtry=2)
    m <- model.frame(terms(reformulate(attributes(Terms)$term.labels)),
                     data.frame(m))
    ## if (!is.null(y)) m <- m[, -1, drop=FALSE]
    for (i in seq(along=m)) {
        if (is.ordered(m[[i]])) m[[i]] <- as.numeric(m[[i]])
    }
    ret <- randomForest.default(m, y, ...)
    cl <- match.call()
    cl[[1]] <- as.name("randomForest")
    ret$call <- cl
    ret$terms <- Terms
    if (!is.null(attr(y, "na.action"))) {
        attr(ret$predicted, "na.action") <- ret$na.action <- attr(y, "na.action")
	}
    class(ret) <- c("randomForest.formula", "randomForest")
    return(ret)
}
rfcv <- function(trainx, trainy, cv.fold=5, scale="log", step=0.5,
                 mtry=function(p) max(1, floor(sqrt(p))), recursive=FALSE,
                 ...) {
    classRF <- is.factor(trainy)
    n <- nrow(trainx)
    p <- ncol(trainx)
    if (scale == "log") {
        k <- floor(log(p, base=1/step))
        n.var <- round(p * step^(0:(k-1)))
        same <- diff(n.var) == 0
        if (any(same)) n.var <- n.var[-which(same)]
        if (! 1 %in% n.var) n.var <- c(n.var, 1)
    } else {
        n.var <- seq(from=p, to=1, by=step)
    }
    k <- length(n.var)
    cv.pred <- vector(k, mode="list")
    for (i in 1:k) cv.pred[[i]] <- trainy
    ## Generate the indices of the splits
    ## Stratify the classes for classification problem.
    ## For regression, bin the response into 5 bins and stratify.
    if(classRF) {
        f <- trainy
    } else {
        ##f <- cut(trainy, c(-Inf, quantile(trainy, 1:4/5), Inf))
		f <- factor(rep(1:5, length=length(trainy))[order(order(trainy))])
    }
    nlvl <- table(f)
    idx <- numeric(n)
    for (i in 1:length(nlvl)) {
        idx[which(f == levels(f)[i])] <-  sample(rep(1:cv.fold, length=nlvl[i]))
    }

    for (i in 1:cv.fold) {
        ## cat(".")
        all.rf <- randomForest(trainx[idx != i, , drop=FALSE],
                               trainy[idx != i],
                               trainx[idx == i, , drop=FALSE],
                               trainy[idx == i],
                               mtry=mtry(p), importance=TRUE, ...)
        cv.pred[[1]][idx == i] <- all.rf$test$predicted
        impvar <- (1:p)[order(all.rf$importance[,1], decreasing=TRUE)]
        for (j in 2:k) {
            imp.idx <- impvar[1:n.var[j]]
            sub.rf <-
                randomForest(trainx[idx != i, imp.idx, drop=FALSE],
                             trainy[idx != i],
                             trainx[idx == i, imp.idx, drop=FALSE],
                             trainy[idx == i],
                             mtry=mtry(n.var[j]), importance=recursive, ...)
            cv.pred[[j]][idx == i] <- sub.rf$test$predicted
            ## For recursive selection, use importance measures from the sub-model.
            if (recursive) {
                impvar <-
                    (1:length(imp.idx))[order(sub.rf$importance[,1], decreasing=TRUE)]
      }
      NULL
    }
    NULL
  }
  ## cat("\n")
  if(classRF) {
    error.cv <- sapply(cv.pred, function(x) mean(trainy != x))
  } else {
    error.cv <- sapply(cv.pred, function(x) mean((trainy - x)^2))
  }
  names(error.cv) <- names(cv.pred) <- n.var
  list(n.var=n.var, error.cv=error.cv, predicted=cv.pred)
}
rfImpute <- function(x, ...)
    UseMethod("rfImpute")

rfImpute.formula <- function(x, data, ..., subset) {
    if (!inherits(x, "formula"))
        stop("method is only for formula objects")
    call <- match.call()
    m <- match.call(expand.dots = FALSE)
    names(m)[2] <- "formula"
    if (is.matrix(eval(m$data, parent.frame())))
        m$data <- as.data.frame(data)
    m$... <- NULL
    m$na.action <- as.name("na.pass")
    m[[1]] <- as.name("model.frame")
    m <- eval(m, parent.frame())
    Terms <- attr(m, "terms")
    attr(Terms, "intercept") <- 0
    y <- model.response(m)
    if (!is.null(y)) m <- m[,-1]
    for (i in seq(along=ncol(m))) {
        if(is.ordered(m[[i]])) m[[i]] <- as.numeric(m[[i]])
    }
    ret <- rfImpute.default(m, y, ...)
    names(ret)[1] <- deparse(as.list(x)[[2]])
    ret
}

rfImpute.default <- function(x, y, iter=5, ntree=300, ...) {
    if (any(is.na(y))) stop("Can't have NAs in", deparse(substitute(y)))
    if (!any(is.na(x))) stop("No NAs found in ", deparse(substitute(x)))
    xf <- na.roughfix(x)
    hasNA <- which(apply(x, 2, function(x) any(is.na(x))))
    if (is.data.frame(x)) {
        isfac <- sapply(x, is.factor)
    } else {
        isfac <- rep(FALSE, ncol(x))
    }
    
    for (i in 1:iter) {
        prox <- randomForest(xf, y, ntree=ntree, ..., do.trace=ntree,
                             proximity=TRUE)$proximity
        for (j in hasNA) {
            miss <- which(is.na(x[, j]))
            if (isfac[j]) {
                lvl <- levels(x[[j]])
                catprox <- apply(prox[-miss, miss, drop=FALSE], 2,
                                 function(v) lvl[which.max(tapply(v, x[[j]][-miss], mean))])
                xf[miss, j] <- catprox
            } else {
                sumprox <- colSums(prox[-miss, miss, drop=FALSE])
                xf[miss, j] <- (prox[miss, -miss, drop=FALSE] %*% xf[,j][-miss]) / (1e-8 + sumprox)
            }
            NULL
        }
    }
    xf <- cbind(y, xf)
    names(xf)[1] <- deparse(substitute(y))
    xf
}
rfNews <- function() {
    newsfile <- file.path(system.file(package="randomForest"), "NEWS")
    file.show(newsfile)
}
treesize <- function(x, terminal=TRUE) {
  if(!inherits(x, "randomForest"))
    stop("This function only works for objects of class `randomForest'")
  if(is.null(x$forest)) stop("The object must contain the forest component")
  if(terminal) return((x$forest$ndbigtree+1)/2) else return(x$forest$ndbigtree)
}
tuneRF <- function(x, y, mtryStart=if(is.factor(y)) floor(sqrt(ncol(x))) else
                   floor(ncol(x)/3), ntreeTry=50, stepFactor=2,
                   improve=0.05, trace=TRUE, plot=TRUE, doBest=FALSE, ...) {
  if (improve < 0) stop ("improve must be non-negative.")
  classRF <- is.factor(y)
  errorOld <- if (classRF) {
    randomForest(x, y, mtry=mtryStart, ntree=ntreeTry,
                 keep.forest=FALSE, ...)$err.rate[ntreeTry,1]
  } else {
    randomForest(x, y, mtry=mtryStart, ntree=ntreeTry,
                 keep.forest=FALSE, ...)$mse[ntreeTry]
  }
  if (errorOld < 0) stop("Initial setting gave 0 error and no room for improvement.")
  if (trace) {
    cat("mtry =", mtryStart, " OOB error =",
        if (classRF) paste(100*round(errorOld, 4), "%", sep="") else
        errorOld, "\n")
  }

  oobError <- list()
  oobError[[1]] <- errorOld
  names(oobError)[1] <- mtryStart  
  
  for (direction in c("left", "right")) {
    if (trace) cat("Searching", direction, "...\n")
    Improve <- 1.1*improve
    mtryBest <- mtryStart
    mtryCur <- mtryStart
    while (Improve >= improve) {
      mtryOld <- mtryCur
      mtryCur <- if (direction == "left") {
        max(1, ceiling(mtryCur / stepFactor))
      } else {
        min(ncol(x), floor(mtryCur * stepFactor))
      }
      if (mtryCur == mtryOld) break
      errorCur <- if (classRF) {
        randomForest(x, y, mtry=mtryCur, ntree=ntreeTry,
                     keep.forest=FALSE, ...)$err.rate[ntreeTry,"OOB"]
      } else {
        randomForest(x, y, mtry=mtryCur, ntree=ntreeTry,
                     keep.forest=FALSE, ...)$mse[ntreeTry]
      }
      if (trace) {
        cat("mtry =",mtryCur, "\tOOB error =",
            if (classRF) paste(100*round(errorCur, 4), "%", sep="") else
            errorCur, "\n")
      }
      oobError[[as.character(mtryCur)]] <- errorCur
      Improve <- 1 - errorCur/errorOld
      cat(Improve, improve, "\n")
      if (Improve > improve) {
        errorOld <- errorCur
        mtryBest <- mtryCur
      }
    }
  }
  mtry <- sort(as.numeric(names(oobError)))
  res <- unlist(oobError[as.character(mtry)])
  res <- cbind(mtry=mtry, OOBError=res)

  if (plot) {
    plot(res, xlab=expression(m[try]), ylab="OOB Error", type="o", log="x",
         xaxt="n")
    axis(1, at=res[,"mtry"])
  }

  if (doBest) 
    res <- randomForest(x, y, mtry=res[which.min(res[,2]), 1], ...)
  
  res
}
varUsed <- function(x, by.tree=FALSE, count=TRUE) {
    if (!inherits(x, "randomForest"))
        stop(deparse(substitute(x)), "is not a randomForest object")
    if (is.null(x$forest))
        stop(deparse(substitute(x)), "does not contain forest")
    
    p <- length(x$forest$ncat)  # Total number of variables.
    if (count) {
        if (by.tree) {
            v <- apply(x$forest$bestvar, 2, function(x) {
                xx <- numeric(p)
                y <- table(x[x>0])
                xx[as.numeric(names(y))] <- y
                xx
            })
        } else {
            v <- numeric(p)
            vv <- table(x$forest$bestvar[x$forest$bestvar > 0])
            v[as.numeric(names(vv))] <- vv
        }
    } else {
        v <- apply(x$forest$bestvar, 2, function(x) sort(unique(x[x>0])))
        if(!by.tree) v <- sort(unique(unlist(v)))
    }
    v
}
.onAttach <- function(libname, pkgname) {
    RFver <- read.dcf(file=system.file("DESCRIPTION", package=pkgname),
                      fields="Version")
    packageStartupMessage(paste(pkgname, RFver))
    packageStartupMessage("Type rfNews() to see new features/changes/bug fixes.")
}
