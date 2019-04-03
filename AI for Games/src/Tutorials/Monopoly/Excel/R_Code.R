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

#Plot some stuff
MDI %>% filter(MDI$Game > 500) %>% 
  ggplot(aes(x=Game, y=Died.At, col=Place, size=Total.Asset.Value))+
  geom_point(alpha = 0.3)+geom_smooth(method = lm)+facet_wrap(~Player)

#Check stuff
df1 <- MDI %>% filter(MDI$Game < 300) %>% dplyr::select(Player, Place, Died.At, Total.Asset.Value)
t.test(data = df1, Died.At ~ (Player == 0 | Player == 1))
df1 <- MDI %>% filter(MDI$Game > 300, MDI$Game < 1000) %>% dplyr::select(Player, Place, Died.At, Total.Asset.Value)
t.test(data = df1, Died.At ~ (Player == 0 | Player == 1))
df1 <- MDI %>% filter(MDI$Game > 1000) %>% dplyr::select(Player, Place, Died.At, Total.Asset.Value)
t.test(data = df1, Died.At ~ (Player == 0 | Player == 1))

df1 <- MDI %>% filter(MDI$Place == 1) %>% dplyr::select(Player, Place, Died.At, Total.Asset.Value)


for (i in 0:3) {
  print(sum(MDI$Player == i & MDI$Place == 1))
}
for (i in 0:3) {
  print(sum(MDI_No_AI$Player == i & MDI_No_AI$Place == 1))
}
#t.test(MDI$Died.At ~ MDI$Place) #, paired = F, var.equal = T)
#t.test(data = MDI, Died.At ~ Place)
#t.test(data = MDI, Died.At ~ (Player == 0 | Player == 1))


PlaceSum <- data.frame(
  "Player" = 1:4, 
  "First.Place" = c(
    sum(MDI$Player == 0 & MDI$Place == 1),
    sum(MDI$Player == 1 & MDI$Place == 1),
    sum(MDI$Player == 2 & MDI$Place == 1),
    sum(MDI$Player == 3 & MDI$Place == 1)
    ),
  "Second.Place" = c(
    sum(MDI$Player == 0 & MDI$Place == 2),
    sum(MDI$Player == 1 & MDI$Place == 2),
    sum(MDI$Player == 2 & MDI$Place == 2),
    sum(MDI$Player == 3 & MDI$Place == 2)
  ),
  "Third.Place" = c(
    sum(MDI$Player == 0 & MDI$Place == 3),
    sum(MDI$Player == 1 & MDI$Place == 3),
    sum(MDI$Player == 2 & MDI$Place == 3),
    sum(MDI$Player == 3 & MDI$Place == 3)
  ),
  "Forth.Place" = c(
    sum(MDI$Player == 0 & MDI$Place == 4),
    sum(MDI$Player == 1 & MDI$Place == 4),
    sum(MDI$Player == 2 & MDI$Place == 4),
    sum(MDI$Player == 3 & MDI$Place == 4)
  )
)



