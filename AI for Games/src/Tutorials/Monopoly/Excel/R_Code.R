if (!require(dplyr)) install.packages("dplyr")
library(dplyr)
if (!require(ggplot2)) install.packages("ggplot2")
library(ggplot2)
if(!require(Rcmdr)) install.packages("Rcmdr")
library(Rcmdr)
if(!require(ggpubr)) install.packages("ggpubr")
library(ggpubr)
if(!require(gplots)) install.packages("gplots")
library(gplots)

#Monopoly Data Total
MDT <- read.csv("10000_AI_Total.csv")
MDT2 <- read.csv("10000v2_AI_Total.csv")
MDT3 <- read.csv("10000v3_AI_Total.csv")
MDT4 <- read.csv("10000v4_AI_Total.csv")
MDT_No_AI <- read.csv("10000_No_AI_Total.csv")
#Monopoly Data Individual Rounds
MDI <- read.csv("10000_AI_Individual_Rounds.csv")
MDI2 <- read.csv("10000v2_AI_Individual_Rounds.csv")
MDI3 <- read.csv("10000v3_AI_Individual_Rounds.csv")
MDI4 <- read.csv("10000v4_AI_Individual_Rounds.csv")
MDI_No_AI <- read.csv("10000_No_AI_Individual_Rounds.csv")


p1 <- filter(MDI, Player == 0)
p2 <- filter(MDI, Player == 1)
p3 <- filter(MDI, Player == 2)
p4 <- filter(MDI, Player == 3)

#Summary of Monopoly Total
summary(MDT)
#Summary of Monopoly Data Individual Rounds
summary(MDI)
#summary(p1)
#summary(p2)
#summary(p3)
#summary(p4)

#plot(MDI)
hist(MDI$Times.Around.Board)
par(mfrow=c(2,2))
hist(p1$Times.Around.Board)
hist(p2$Times.Around.Board)
hist(p3$Times.Around.Board)
hist(p4$Times.Around.Board)

par(mfrow=c(2,3))
boxplot(data = MDI, Place~Player, ylab = "Place", xlab = "Player")
boxplot(data = MDI, Died.At~Player, ylab = "Died at", xlab = "Player")
boxplot(data = MDI, Times.Around.Board~Player, ylab = "Times Around Board", xlab = "Player")
boxplot(data = MDI, Total.Asset.Value~Player, ylab = "Total Asset Value", xlab = "Player")
boxplot(data = MDI, Hotels~Player, ylab = "Hotels", xlab = "Player")
boxplot(data = MDI, Houses~Player, ylab = "Houses", xlab = "Player")
#boxplot(data = MDI, Money~Player, ylab = "Money", xlab = "Player")
par(mfrow=c(1,1))

#Plot some stuff
MDI %>% filter(MDI$Game > 500) %>% 
  ggplot(aes(x=Game, y=Died.At, col=Place, size=Total.Asset.Value))+
  geom_point(alpha = 0.3)+geom_smooth(method = lm)+facet_wrap(~Player)

MDI %>% 
  ggplot(aes(x=Times.Around.Board, y=Total.Asset.Value, col=Place))+
  geom_point(alpha = 0.3)+geom_smooth(method = lm)+facet_wrap(~Player)

GetSumByPlayer <- function(data){
  result <- data.frame(
    "Value" = 1:4, 
    "First" = c(
      sum(data$Player == 0 & data$Place == 1),
      sum(data$Player == 1 & data$Place == 1),
      sum(data$Player == 2 & data$Place == 1),
      sum(data$Player == 3 & data$Place == 1) ),
    "Second" = c(
      sum(data$Player == 0 & data$Place == 2),
      sum(data$Player == 1 & data$Place == 2),
      sum(data$Player == 2 & data$Place == 2),
      sum(data$Player == 3 & data$Place == 2) ),
    "Third" = c(
      sum(data$Player == 0 & data$Place == 3),
      sum(data$Player == 1 & data$Place == 3),
      sum(data$Player == 2 & data$Place == 3),
      sum(data$Player == 3 & data$Place == 3) ),
    "Forth" = c(
      sum(data$Player == 0 & data$Place == 4),
      sum(data$Player == 1 & data$Place == 4),
      sum(data$Player == 2 & data$Place == 4),
      sum(data$Player == 3 & data$Place == 4) )
  )
  return (result);
}
GetSumByPlace <- function(data){
  result <- data.frame(
    "Value" = 1:4, 
    "First" = c(
      sum(data$Player == 0 & data$Place == 1),
      sum(data$Player == 0 & data$Place == 2),
      sum(data$Player == 0 & data$Place == 3),
      sum(data$Player == 0 & data$Place == 4) ),
    "Second" = c(
      sum(data$Player == 1 & data$Place == 1),
      sum(data$Player == 1 & data$Place == 2),
      sum(data$Player == 1 & data$Place == 3),
      sum(data$Player == 1 & data$Place == 4) ),
    "Third" = c(
      sum(data$Player == 2 & data$Place == 1),
      sum(data$Player == 2 & data$Place == 2),
      sum(data$Player == 2 & data$Place == 3),
      sum(data$Player == 2 & data$Place == 4) ),
    "Forth" = c(
      sum(data$Player == 3 & data$Place == 1),
      sum(data$Player == 3 & data$Place == 2),
      sum(data$Player == 3 & data$Place == 3),
      sum(data$Player == 3 & data$Place == 4) )
  )
  return (result);
}

PlayerSum <- GetSumByPlayer(MDI)
PlayerSum2 <- GetSumByPlayer(MDI2)
PlayerSum3 <- GetSumByPlayer(MDI3)
PlayerSum_No_AI <- GetSumByPlayer(MDI_No_AI)
PlaceSum <- GetSumByPlace(MDI)
PlaceSum2 <- GetSumByPlace(MDI2)
PlaceSum3 <- GetSumByPlace(MDI3)
PlaceSum_No_AI <- GetSumByPlayer(MDI_No_AI)

PlayerSum
PlayerSum2
PlayerSum3
PlayerSum_No_AI


t.test(data = PlayerSum,  First ~ (Value == 1 | Value == 4))#Almost a significant difference
t.test(data = PlayerSum2, First ~ (Value == 1 | Value == 4))#Not a significant difference
t.test(data = PlaceSum,   First ~ (Value == 1 | Value == 4))#Not a significant difference
t.test(data = PlaceSum2,  First ~ (Value == 1 | Value == 4))#Not a significant difference
#Everything else was good

shapiro.test(PlayerSum$First)  # Didn't
shapiro.test(PlayerSum$Second) # Almost
shapiro.test(PlayerSum$Third)  # Didn't
shapiro.test(PlayerSum$Forth)  # Didn't

shapiro.test(PlayerSum2$First)  # Didn't
shapiro.test(PlayerSum2$Second) # Did
shapiro.test(PlayerSum2$Third)  # Didn't
shapiro.test(PlayerSum2$Forth)  # Didn't

shapiro.test(PlaceSum$First)  # Didn't
shapiro.test(PlaceSum$Second) # Did
shapiro.test(PlaceSum$Third)  # Didn't
shapiro.test(PlaceSum$Forth)  # Didn't

shapiro.test(PlaceSum2$First)  # Didn't
shapiro.test(PlaceSum2$Second) # Almost
shapiro.test(PlaceSum2$Third)  # Didn't
shapiro.test(PlaceSum2$Forth)  # Almost

bartlett.test(PlayerSum)  # not homogenous
bartlett.test(PlayerSum2) # not homogenous
bartlett.test(PlaceSum)   # not homogenous
bartlett.test(PlaceSum2)  # not homogenous


#Just using PlaceSum here
anovaRes <- aov(PlaceSum$First~PlaceSum$Value)  #A significant difference
summary(anovaRes)
anovaRes <- aov(PlaceSum$Second~PlaceSum$Value) #Not a significant difference
summary(anovaRes)
anovaRes <- aov(PlaceSum$Third~PlaceSum$Value)  #Not a significant difference
summary(anovaRes)
anovaRes <- aov(PlaceSum$Forth~PlaceSum$Value)  #A significant difference
summary(anovaRes)

anovaRes <- aov(PlaceSum2$First~PlaceSum2$Value)  #A significant difference
summary(anovaRes)
anovaRes <- aov(PlaceSum2$Second~PlaceSum2$Value) #Not a significant difference
summary(anovaRes)
anovaRes <- aov(PlaceSum2$Third~PlaceSum2$Value)  #Not a significant difference
summary(anovaRes)
anovaRes <- aov(PlaceSum2$Forth~PlaceSum2$Value)  #Not a significant difference
summary(anovaRes)


# 1. Homogeneity of variances
# 2. Normality

# Compute the analysis of variance
res.aov <- aov(Place ~ Player, data = MDI)
summary(res.aov) # Summary of the analysis
par(mfrow=c(2,3))
for (i in 1:6) {
  plot(res.aov, i)
}
res.aov <- aov(Died.At ~ Player, data = MDI)
summary(res.aov) # Summary of the analysis
par(mfrow=c(2,3))
for (i in 1:6) {
  plot(res.aov, i)
}
res.aov <- aov(Total.Asset.Value ~ Player, data = MDI)
summary(res.aov) # Summary of the analysis
par(mfrow=c(2,3))
for (i in 1:6) {
  plot(res.aov, i)
}
par(mfrow=c(1,1))

ggboxplot(MDI, x = "Player", y = "Place", 
          color = "Player", palette = c(2:5),
          order = c("0", "1", "2", "3"),
          ylab = "Place", xlab = "Player")
ggline(MDI, x = "Player", y = "Place", 
       add = c("mean_sd"), 
       order = c("0", "1", "2", "3"),
       ylab = "Place", xlab = "Player")
ggline(MDI, x = "Player", y = "Place", 
       add = c("mean_se"), #mean_ci
       order = c("0", "1", "2", "3"),
       ylab = "Place", xlab = "Player")
plotmeans(Place ~ Player, data = MDI, frame = FALSE,
          xlab = "Player", ylab = "Place",
          main="Mean Plot with 95% CI") 







