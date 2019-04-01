#Monopoly Data Total
MDT <- read.csv("10000_Total.csv")
#Monopoly Data Individual Rounds
MDI <- read.csv("10000_Individual_Rounds.csv")

#Summary of Monopoly Total
#The data needs to be rearranged as is doesn't give good results 
summary(MDT)
#Summary of Monopoly Data Individual Rounds
summary(MDI)

if (!require(dplyr)) install.packages("dplyr")
library(dplyr)

Players.p <- filter(MDI, Player == 1)
p1 <- summary(filter(MDI, Player == 0))
p2 <- summary(filter(MDI, Player == 1))
p3 <- summary(filter(MDI, Player == 2))
p4 <- summary(filter(MDI, Player == 3))

if (!require(ggplot2)) install.packages("ggplot2")
library(ggplot2)

