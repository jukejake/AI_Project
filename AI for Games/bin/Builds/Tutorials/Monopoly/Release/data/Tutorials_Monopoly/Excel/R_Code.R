if (!require(dplyr)) install.packages("dplyr")
library(dplyr)
if (!require(ggplot2)) install.packages("ggplot2")
library(ggplot2)
if(!require(Rcmdr)) install.packages("Rcmdr")
library(Rcmdr)

#Monopoly Data Total
MDT <- read.csv("10000_AI_Total.csv")
MDT_No_AI <- read.csv("10000_No_AI_Total.csv")
#Monopoly Data Individual Rounds
MDI <- read.csv("10000_AI_Individual_Rounds.csv")
MDI_No_AI <- read.csv("10000_No_AI_Individual_Rounds.csv")

#Summary of Monopoly Total
#The data needs to be rearranged as is doesn't give good results 
summary(MDT)
#Summary of Monopoly Data Individual Rounds
summary(MDI)

p1 <- filter(MDI, Player == 0)
p2 <- filter(MDI, Player == 1)
p3 <- filter(MDI, Player == 2)
p4 <- filter(MDI, Player == 3)

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

#Plot some stuff
MDI %>% filter(MDI$Game > 500) %>% 
  ggplot(aes(x=Game, y=Died.At, col=Place, size=Total.Asset.Value))+
  geom_point(alpha = 0.3)+geom_smooth(method = lm)+facet_wrap(~Player)


PSum <- data.frame(
  "Player" = 1:4, 
  "First" = c(
    sum(MDI$Player == 0 & MDI$Place == 1),
    sum(MDI$Player == 1 & MDI$Place == 1),
    sum(MDI$Player == 2 & MDI$Place == 1),
    sum(MDI$Player == 3 & MDI$Place == 1) ),
  "Second" = c(
    sum(MDI$Player == 0 & MDI$Place == 2),
    sum(MDI$Player == 1 & MDI$Place == 2),
    sum(MDI$Player == 2 & MDI$Place == 2),
    sum(MDI$Player == 3 & MDI$Place == 2) ),
  "Third" = c(
    sum(MDI$Player == 0 & MDI$Place == 3),
    sum(MDI$Player == 1 & MDI$Place == 3),
    sum(MDI$Player == 2 & MDI$Place == 3),
    sum(MDI$Player == 3 & MDI$Place == 3) ),
  "Forth" = c(
    sum(MDI$Player == 0 & MDI$Place == 4),
    sum(MDI$Player == 1 & MDI$Place == 4),
    sum(MDI$Player == 2 & MDI$Place == 4),
    sum(MDI$Player == 3 & MDI$Place == 4) )
)
PSum2 <- data.frame(
  "Place" = 1:4, 
  "First" = c(
    sum(MDI$Player == 0 & MDI$Place == 1),
    sum(MDI$Player == 0 & MDI$Place == 2),
    sum(MDI$Player == 0 & MDI$Place == 3),
    sum(MDI$Player == 0 & MDI$Place == 4) ),
  "Second" = c(
    sum(MDI$Player == 1 & MDI$Place == 1),
    sum(MDI$Player == 1 & MDI$Place == 2),
    sum(MDI$Player == 1 & MDI$Place == 3),
    sum(MDI$Player == 1 & MDI$Place == 4) ),
  "Third" = c(
    sum(MDI$Player == 2 & MDI$Place == 1),
    sum(MDI$Player == 2 & MDI$Place == 2),
    sum(MDI$Player == 2 & MDI$Place == 3),
    sum(MDI$Player == 2 & MDI$Place == 4) ),
  "Forth" = c(
    sum(MDI$Player == 3 & MDI$Place == 1),
    sum(MDI$Player == 3 & MDI$Place == 2),
    sum(MDI$Player == 3 & MDI$Place == 3),
    sum(MDI$Player == 3 & MDI$Place == 4) )
)
PSum_No_AI <- data.frame(
  "Player" = 1:4, 
  "First" = c(
    sum(MDI_No_AI$Player == 0 & MDI_No_AI$Place == 1),
    sum(MDI_No_AI$Player == 1 & MDI_No_AI$Place == 1),
    sum(MDI_No_AI$Player == 2 & MDI_No_AI$Place == 1),
    sum(MDI_No_AI$Player == 3 & MDI_No_AI$Place == 1) ),
  "Second" = c(
    sum(MDI_No_AI$Player == 0 & MDI_No_AI$Place == 2),
    sum(MDI_No_AI$Player == 1 & MDI_No_AI$Place == 2),
    sum(MDI_No_AI$Player == 2 & MDI_No_AI$Place == 2),
    sum(MDI_No_AI$Player == 3 & MDI_No_AI$Place == 2) ),
  "Third" = c(
    sum(MDI_No_AI$Player == 0 & MDI_No_AI$Place == 3),
    sum(MDI_No_AI$Player == 1 & MDI_No_AI$Place == 3),
    sum(MDI_No_AI$Player == 2 & MDI_No_AI$Place == 3),
    sum(MDI_No_AI$Player == 3 & MDI_No_AI$Place == 3) ),
  "Forth" = c(
    sum(MDI_No_AI$Player == 0 & MDI_No_AI$Place == 4),
    sum(MDI_No_AI$Player == 1 & MDI_No_AI$Place == 4),
    sum(MDI_No_AI$Player == 2 & MDI_No_AI$Place == 4),
    sum(MDI_No_AI$Player == 3 & MDI_No_AI$Place == 4) )
)
PSum
PSum_No_AI
summary(PSum)
summary(PSum_No_AI)

t.test(data = PSum, First ~ (Player == 1 | Player == 4))#Almost a significant difference
t.test(data = PSum, Third ~ (Player == 1 | Player == 4))#A significant difference
#Everything else was good

shapiro.test(PSum$First)  # passed normality test
shapiro.test(PSum$Second) # just passed normality test
shapiro.test(PSum$Third)  # passed normality test
shapiro.test(PSum$Forth)  # passed normality test

bartlett.test(PSum) # not homogenous

anovaRes <- aov(PSum2$First~PSum2$Place)  #A significant difference
summary(anovaRes)
anovaRes <- aov(PSum2$Second~PSum2$Place) #Not a significant difference
summary(anovaRes)
anovaRes <- aov(PSum2$Third~PSum2$Place)  #Not a significant difference
summary(anovaRes)
anovaRes <- aov(PSum2$Forth~PSum2$Place)  #A significant difference
summary(anovaRes)




