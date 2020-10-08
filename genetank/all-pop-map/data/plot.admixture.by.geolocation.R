# The rworldmap library loads a higher-reolution world map.  The ggmap
# library is used for the geocoding functionality. The mapplots
# library is used to overlay pie charts on a map.
library("rworldmap") 
library("ggmap") 
library("mapplots") 

# LOAD DATA
# ---------
# Load the admixture proportions, the sample ids and population
# labels, and the geographic annotations.
admix     <- read.table("ancestry-report-data/1kg_hgdp.7.Q")
labels    <- read.table("ancestry-report-data/1kg_hgdp.lab",header = TRUE)
lab_loc   <- read.table("ancestry-report-data/1kg.lab.loc",header = TRUE,sep = "\t",
                        stringsAsFactors = FALSE)
lab_admix <- cbind(labels,admix)
K         <- ncol(admix)
# K <- 5

# Use function geocode from the ggmap package to retrieve
# latitude-longitude pairs from the Google API.
lab_loc <- cbind(lab_loc,geocode(lab_loc$loc))

# COMPILE SUMMARY STATISTICS FROM DATA
# ------------------------------------
# Create a table containing the latitude-longitude pair and admixture
# proportions for each sample.
lab_latlon_admix <-
  merge(lab_loc,lab_admix,by = "label")[c("label","lat","lon",paste0("V",1:K))]

# Calculate the average admixture proportion for each population.
x <- aggregate(lab_latlon_admix[,2:10],
               by = list(lab_latlon_admix$label),
               FUN = mean)

# GENERATE WORLD MAP
# ------------------
# Fetching the map data.
mp <- getMap(resolution = "high")

# Draw the map.
dev.new(height = 6,width = 11)
worldMapPlot <- mapCountryData(mp,colourPalette = rep("white",7),borderCol = "gray",
               mapTitle = "mean admixture proportions by map location",
               addLegend = FALSE)

worldMapPlot

# Color Scheme
colorScheme <- c("darkgreen","deepskyblue","firebrick3","darkgoldenrod1","darkgray")

# Overlay pie charts on the map.
n <- nrow(x)
for (i in 1:n)
  add.pie(z = as.numeric(x[i,4:10]),y = x[i,]$lat,x = x[i,]$lon,
          radius = 3,labels = x[i,1],col = colorScheme,label.dist = 1.5,
          cex = 0.65,border = NA,lwd = 0.1)

# Add the legend to the diagram.
# Ethnicities: http://www.internationalgenome.org/category/population/
legtext <- c("European", "African", "East Asian", "South Asian", "Ad Mixed American")
xcoords <- c(0, 40, 80, 120, 160)
secondvector <- (1:length(legtext))-1
textwidths <- xcoords/secondvector # this works for all but the first element
textwidths[1] <- 0 
legend(x = -190,y = -50,col = colorScheme,legend = legtext,pch = 20,horiz = TRUE,
       box.col = "white",bg = "white",cex = 1, text.width=textwidths)
