  char test_R_code[]="x <- 2
  y <- 3
  x <- x + y
  y <- x * y
  a <- x-y
  b <- x/y
  a
  b


  # ---Data Structures---

  # Vectors
  workshop <- c(1, 2, 1, 2, 1, 2, 1, 2)
  workshop

  gender <- c(\"f\", \"f\", \"f\", NA, \"m\", \"m\", \"m\", \"m\")
  q1 <- c(1, 2, 2, 3, 4, 5, 5, 4)
  q2 <- c(1, 1, 2, 1, 5, 4, 3, 5)
  q3 <- c(5, 4, 4,NA, 2, 5, 4, 5)
  q4 <- c(1, 1, 3, 3, 4, 5, 4, 5)

  # Selecting Elements of Vectors
  q1[5]
  q1[ c(5, 6, 7, 8) ]
  q1[5:8]
  q1[gender == \"m\"]
  max( q1[ gender == \"m\" ], na.rm = TRUE)

   # ---Writing Your Own Functions (Macros)---

  myvar <- c(1, 2, 3, 4, 5)

  # A function with vector output.
  mystats  <- function(x) {
  mymax <- max(x, na.rm = TRUE)
  c(input = x, max = mymax)
  }
  mystats(myvar)
  myVector <- mystats(myvar)
  myVector

  # A function with list output.
  mystats   <- function(x) {
  myinput <- x
  mymax  <- max(x, na.rm = TRUE)
  list(data = myinput, max = mymax)
  }
  mystats(myvar)
  myStatlist <- mystats(myvar)
  myStatlist
\n";

char share_base_r[]="\
.Internal(eval(quote({ sep =\" \"\n\
..lazyLoad <- function(filebase, envir = parent.frame())\n\
{\n\
    ##\n\
    ## bootstrapping definitions so we can load base\n\
    ##\n\
    glue <- function (..., sep =\" \", collapse = NULL)\n\
        .Internal(paste(list(...), sep, collapse))\n\
    readRDS <- function (file) {\n\
        halt <- function (message) .Internal(stop(TRUE, message))\n\
        gzfile <- function (description, open)\n\
            .Internal(gzfile(description, open, \"\", 6))\n\
        close <- function (con) .Internal(close(con, \"rw\"))\n\
        if (! is.character(file)) halt(\"bad file name\")\n\
        con <- gzfile(file, \"rb\")\n\
        on.exit(close(con))\n\
        .Internal(unserializeFromConn(con, baseenv()))\n\
    }\n\
    `parent.env<-` <-\n\
        function (env, value) .Internal(`parent.env<-`(env, value))\n\
    existsInFrame <- function (x, env) .Internal(exists(x, env, \"any\", FALSE))\n\
    getFromFrame <- function (x, env) .Internal(get(x, env, \"any\", FALSE))\n\
    set <- function (x, value, env) .Internal(assign(x, value, env, FALSE))\n\
    environment <- function () .Internal(environment(NULL))\n\
    mkenv <- function() .Internal(new.env(TRUE, baseenv(), 29L))\n\
\n\
    ##\n\
    ## main body\n\
    ##\n\
    mapfile <- glue(filebase, \"rdx\", sep = \".\")\n\
    datafile <- glue(filebase, \"rdb\", sep = \".\")\n\
    env <- mkenv()\n\
    map <- readRDS(mapfile)\n\
    vars <- names(map$variables)\n\
    rvars <- names(map$references)\n\
    compressed <- map$compressed\n\
    for (i in seq_along(rvars))\n\
        set(rvars[i], map$references[[i]], env)\n\
    envenv <- mkenv()\n\
    envhook <- function(n) {\n\
        if (existsInFrame(n, envenv))\n\
            getFromFrame(n, envenv)\n\
        else {\n\
            e <- mkenv()\n\
            set(n, e, envenv)           # MUST do this immediately\n\
            key <- getFromFrame(n, env)\n\
            data <- lazyLoadDBfetch(key, datafile, compressed, envhook)\n\
            if (is.null(data$enclos))\n\
                parent.env(e) <- emptyenv()\n\
            else\n\
                parent.env(e) <- data$enclos\n\
            vars <- names(data$bindings)\n\
            for (i in seq_along(vars))\n\
                set(vars[i], data$bindings[[i]], e)\n\
            if (! is.null(data$attributes))\n\
                attributes(e) <- data$attributes\n\
            ## there are no S4 objects in base\n\
            if (! is.null(data$locked) && data$locked)\n\
                .Internal(lockEnvironment(e, FALSE))\n\
            e\n\
        }\n\
    }\n\
    expr <- quote(lazyLoadDBfetch(key, datafile, compressed, envhook))\n\
    this <- environment()\n\
    .Internal(makeLazy(vars, map$variables, expr, this, envir))\n\
\n\
    ## reduce memory use\n\
    map <- NULL\n\
    vars <- NULL\n\
    rvars <- NULL\n\
    mapfile <- NULL\n\
    readRDS <- NULL\n\
}\n\
\n\
    existsInBase <- function (x)\n\
        .Internal(exists(x, .BaseNamespaceEnv, \"any\", TRUE))\n\
    glue <- function (..., sep = \" \", collapse = NULL)\n\
        .Internal(paste(list(...), sep, collapse))\n\
\n\
    basedb <- \"/home/shifa/eclipse-workspace/R-3.4.3/bin/exec/base\"\n\
\n\
    ..lazyLoad(basedb, baseenv())\n\
\n\
}), .Internal(new.env(FALSE, baseenv(), 29L)), baseenv()))\n\
\n\
## keep in sync with R/zzz.R\n\
as.numeric <- as.double\n\
is.name <- is.symbol\n\
\n\
\n\
## populate C/Fortran symbols\n\
####local({\n\
####    routines <- getDLLRegisteredRoutines(\"base\")\n\
####    for (i in c(\"dchdc\", # chol, deprecated\n\
####                \"dqrcf\", \"dqrdc2\", \"dqrqty\", \"dqrqy\", \"dqrrsd\", \"dqrxb\", # qr\n\
####                \"dtrco\")) # .kappa_tri\n\
####        assign(paste0(\".F_\", i), routines[[3]][[i]], envir = .BaseNamespaceEnv)\n\
####    for(i in 1:2)\n\
####        lapply(routines[[i]],\n\
####               function(sym) assign(paste0(\".C_\", sym$name), sym, envir = .BaseNamespaceEnv))\n\
####})\n\
\n\
## make sure these two promises are forced to avoid recursive invocation\n\
## of \"args\" and consequent \"promise already under evaluation\" error\n\
\n\
invisible(force(.ArgsEnv))\n\
invisible(force(.GenericArgsEnv))\n\
\n\
## also force these condition system callback promises to avoid\n\
## recursive invocation in some rare situations at start-up\n\
invisible(force(.signalSimpleWarning))\n\
invisible(force(.handleSimpleError))\n\
invisible(force(.tryResumeInterrupt))\n";
