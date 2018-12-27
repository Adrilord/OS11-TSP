meanExecPPI = mean(statistics$ExecPPI)
meanExecPLI = mean(statistics$ExecPLI)
meanExecMI = mean(statistics$ExecMI)
meanLengthPPI = mean(statistics$LengthPPI)
meanLengthPLI = mean(statistics$LengthPLI)
meanLengthMI = mean(statistics$LengthMI)
hist(statistics$LengthPPI,nclass=12, 
     main= 'Histogramme longueur circuit PPI',
     xlab= 'Longueur',
     ylab= 'Frequency')
hist(statistics$LengthPLI,nclass=12, 
     main= 'Histogramme longueur circuit PLI',
     xlab= 'Longueur',
     ylab= 'Frequency')
hist(statistics$LengthMI,nclass=12, 
     main= 'Histogramme longueur circuit MI',
     xlab= 'Longueur',
     ylab= 'Frequency'))
scores <- c(c(0,0,0),c(0,0,0),c(0,0,0))
dim(scores) <- c(3,3)
for(n in 1:100) {
  if(statistics$LengthPPI[n] > statistics$LengthPLI[n]) {
    if(statistics$LengthPPI[n] > statistics$LengthMI[n]) {
      scores[1,1] = scores[1,1]+1;
      if(statistics$LengthPLI[n] > statistics$LengthMI[n]) {
        scores[2,2] = scores[2,2]+1;
        scores[3,3] = scores[3,3]+1;
      } else {
        scores[2,3] = scores[2,3]+1;
        scores[3,2] = scores[3,2]+1;
      }
    } else {
      scores[1,2] = scores[1,2]+1;
      scores[2,3] = scores[2,3]+1;
      scores[3,1] = scores[3,1]+1;
    }
  } else {
    if(statistics$LengthPLI[n] > statistics$LengthMI[n]) {
      scores[2,1] = scores[2,1]+1;
      if(statistics$LengthPPI[n] > statistics$LengthMI[n]) {
        scores[1,2] = scores[1,2]+1;
        scores[3,3] = scores[3,3]+1;
      } else {
        scores[1,3] = scores[1,3]+1;
        scores[3,2] = scores[3,2]+1;
      }
    } else {
      scores[1,3] = scores[1,3]+1;
      scores[2,2] = scores[2,2]+1;
      scores[3,1] = scores[3,1]+1;
    }
  }
}