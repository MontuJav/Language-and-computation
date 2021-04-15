function make2DArray(cols, rows){
  let arr = new Array(cols);
  for(let i=0; i < arr.length ; i++){
    arr[i] = new Array(rows);
  }
  return arr; 
}

let grid;
let cols;
let rows;
let resolution = 5;
let proba;

function setup() {
  createCanvas(700, 700);
  cols = width / resolution;
  rows = height / resolution;
  grid = make2DArray(cols, rows);
  for(let i=0; i<cols; i++){
    for(let j=0; j<rows; j++){
      proba=floor(random(20));
      if(proba==10){
        grid[i][j] = 1;
      }
      else{
        grid[i][j] = 0;
      }
    }
  }
}

function draw(){
  background("#31B404");
  
  let proba1=0;
  let proba2=0;
  
  
  
  for(let i=0; i<cols; i++){
    for(let j=0; j<rows; j++){
      let x = i*resolution;
      let y = j*resolution;
      //Infectadas tipo A
      if((grid[i][j] > 0) &&  (grid[i][j] < 5)){
        fill("#FF0000");
        stroke(0);
        rect(x,y,resolution-1,resolution-1);
      }
      //Infectadas tipo B
      else if(grid[i][j]==5){
        fill("#FFFF00");
        stroke(0);
        rect(x,y,resolution-1,resolution-1);
      }
      //Muertas
      else if(grid[i][j]==6){
        fill("#08088A");
        stroke(0);
        rect(x,y,resolution-1,resolution-1);
      }
    }
  }
  
  let next = make2DArray(cols, rows); //nuevo arreglo
  
  //Compute next 
  for(let i=0; i<cols; i++){
    for(let j=0; j<rows; j++){
            
      let state=grid[i][j];
         
      
      //Cuenta vecinos
      let sum = 0;
      let vecinosA = cuentaVecinosA(grid, i, j);
      let vecinosB = cuentaVecinosB(grid, i, j);
      
      //Casos celula saludable
      if(state == 0 && vecinosA>0){
        next[i][j]=1;
      }
      else if(state == 0 && vecinosB>3){
        next[i][j]=1;
      }
      //Casos celula tipo A
      else if((state > 0) &&  (state < 5)){
         next[i][j]=state+1;     
      }
      //Casos celula tipo B
      else if(state==5){
        next[i][j]=6;
      }
      //Casos celula muerta
      else if(state==6){
        proba1=floor(random(100));
        //99% de que reviva
        if(proba1!=50){
          proba2=floor(random(10000));
          if(proba2==5000){
            next[i][j]=1;
          }
          else{
            next[i][j]=0;
          }
        }
      }
      //Causos default
      else{
        next[i][j]=state;
      } 
      }
      
    
  }
  sleep(5000);
  grid = next;
}

function sleep(milliseconds) {
 var start = new Date().getTime();
 for (var i = 0; i < 1e7; i++) {
  if ((new Date().getTime() - start) > milliseconds) {
   break;
  }
 }
}

function cuentaVecinosA(grid, x, y){
  let sum=0;
  let estado=0;
  for(let i=-1; i < 2; i++){
    for(let j=-1; j < 2; j++){
      let col=(x + i +cols) % cols;
      let row=(y + j +rows) % rows;
      estado=grid[col][row];
      if((estado > 0) &&  (estado < 5)){
        sum++;
      }
    }
  }
  if((grid[x][y]>0)&&(grid[x][y]<5)){
    sum=sum-1;
  }
  return sum;
}

function cuentaVecinosB(grid, x, y){
  let sum=0;
  let estado=0;
  for(let i=-1; i < 2; i++){
    for(let j=-1; j < 2; j++){
      let col=(x + i +cols) % cols;
      let row=(y + j +rows) % rows;
      estado=grid[col][row];
      if(estado==5){
        sum++;
      }
      
    }
  }
  if(grid[x][y]==5){
    sum=sum-1;
  }
  return sum;
}
