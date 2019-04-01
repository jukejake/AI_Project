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

p1 <- filter(MDI, Player == 0)
p2 <- filter(MDI, Player == 1)
p3 <- filter(MDI, Player == 2)
p4 <- filter(MDI, Player == 3)

if (!require(ggplot2)) install.packages("ggplot2")
library(ggplot2)

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

MDI %>% filter(MDI$Game > 500) %>% 
  ggplot(aes(x=Game, y=Died.At, col=Place, size=Total.Asset.Value))+
  geom_point(alpha = 0.3)+geom_smooth(method = lm)+facet_wrap(~Player)

#df1 <- MDI %>% dplyr::select(Player, Place, Died.At, Total.Asset.Value)
#t.test(MDI$Died.At ~ MDI$Place) #, paired = F, var.equal = T)
#t.test(data = MDI, Died.At ~ Place)
t.test(data = MDI, Died.At ~ (Player == 0 | Player == 1))


