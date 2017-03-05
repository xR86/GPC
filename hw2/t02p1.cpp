#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

// concoida lui Nicomede (concoida dreptei)
// $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. sau
// $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. unde
// $t \in (-\pi / 2, \pi / 2)$
void Display1() {
   double xmax, ymax, xmin, ymin;
   double a = 1, b = 2;
   double pi = 4 * atan(1);
   double ratia = 0.05;

   // calculul valorilor maxime/minime ptr. x si y
   // aceste valori vor fi folosite ulterior la scalare
   xmax = a - b - 1;
   xmin = a + b + 1;
   ymax = ymin = 0;
   double t;
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = a + b * cos(t);
      xmax = (xmax < x1) ? x1 : xmax;
      xmin = (xmin > x1) ? x1 : xmin;

      x2 = a - b * cos(t);
      xmax = (xmax < x2) ? x2 : xmax;
      xmin = (xmin > x2) ? x2 : xmin;

      y1 = a * tan(t) + b * sin(t);
      ymax = (ymax < y1) ? y1 : ymax;
      ymin = (ymin > y1) ? y1 : ymin;

      y2 = a * tan(t) - b * sin(t);
      ymax = (ymax < y2) ? y2 : ymax;
      ymin = (ymin > y2) ? y2 : ymin;
   }

   xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
   ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

   // afisarea punctelor propriu-zise precedata de scalare
   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP); 
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = (a + b * cos(t)) / xmax;
      x2 = (a - b * cos(t)) / xmax;
      y1 = (a * tan(t) + b * sin(t)) / ymax;
      y2 = (a * tan(t) - b * sin(t)) / ymax;

      glVertex2f(x1,y1);
   }
   glEnd();

   glBegin(GL_LINE_STRIP); 
   for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = (a + b * cos(t)) / xmax;
      x2 = (a - b * cos(t)) / xmax;
      y1 = (a * tan(t) + b * sin(t)) / ymax;
      y2 = (a * tan(t) - b * sin(t)) / ymax;

      glVertex2f(x2,y2);
   }
   glEnd();
}

// graficul functiei 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2() {
   double pi = 4 * atan(1);
   double xmax = 8 * pi;
   double ymax = exp(1.1);
   double ratia = 0.05;

   // afisarea punctelor propriu-zise precedata de scalare
   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP); 
   for (double x = 0; x < xmax; x += ratia) {
      double x1, y1;
      x1 = x / xmax;
      y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

      glVertex2f(x1,y1);
   }
   glEnd();
}

// graficul functiei
// f(x) = { 1, x=0 ; d(x)/x, 1 < x < 100}
void Display3(){
   double ratia = 0.8; //0.8

   glColor3f(1,0.1,0.1); // rosu
   glLineWidth(1);
   //glScalef( 1, 2, 1);
   glBegin(GL_LINE_STRIP); 
   glVertex2f(0,1);
   for (double x = 1; x < 100; x += ratia) {
      double y;
      y = fabs(round(x) - x) / x;
      //printf("%f %f", x, y);
      
      glVertex2f(x/125, y*2);
   }
   glEnd();
}

//melcul lui Pascal
//  x = 2 * (a * cos(t) + b) * cos(t)
//  y = 2 * (a * cos(t) + b) * sin(t)
//  t \in (-pi, pi)
//  a = 0.3, b = 0.2
void Display4(){
   double pi = 4 * atan(1);
   double xmax = 8 * pi;
   double ymax = exp(1.1);
   double ratia = 0.05;

   double a = 0.3;
   double b = 0.2;

   double shift = 0.4;

   // afisarea punctelor propriu-zise precedata de scalare
   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP); 
   for (double t = -1 * pi; t < pi; t += ratia) {
      double x1, y1;
      //x1 = x / xmax;
      x1 = 2 * (a * cos(t) + b) * cos(t);
      //y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;
      y1 = 2 * (a * cos(t) + b) * sin(t);

      glVertex2f(x1-shift,y1);
   }
   glEnd();
}

//trisectoarea lui Longchamps
void Display5(){
   double pi = 4 * atan(1);
   double ratia = 0.005;

   double a = 0.2;

   glColor3f(0.1,0.1,0.9); // rosu
   glBegin(GL_LINE_STRIP); 
   for(double t = -pi/2; t < pi/2; t+= ratia){
      //printf('', t);
      
      double x, y;
      x = a / (4 * pow(cos(t), 2) - 3);
      y = a * tan(t) / (4 * pow(cos(t), 2) - 3);
      if (x <= 0 && y >= 0 && x >=-1 && y <= 1){
         glVertex2d(x,y);
      }
   }
   glEnd();

   double ratia2 = 0.0001;
   //glClear(GL_COLOR_BUFFER_BIT);

   glColor3f(1,0.1,0.1); // rosu
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_TRIANGLES); 
   for(double t = -pi/2; t < pi/2; t+= ratia){
      //printf('', t);
      
      double x, y;
      x = a / (4 * pow(cos(t), 2) - 3);
      y = a * tan(t) / (4 * pow(cos(t), 2) - 3);
      if (x <= 0 && y >= 0 && x >=-1 && y <= 1){
         //if (! (x + ratia == 0 || x - ratia == 0 || 
         //      y + ratia == 0 || y - ratia == 0)){
         if (! (x >= -0.2 && x <= -0.09 ) && 
               y > 0.25 ){
            glVertex2d(-1,1);
            glVertex2d(x,y); //close enough, elimina mijlocul
            //glVertex2d(x - ratia2,y - ratia2);
            //glVertex2d(x + ratia2,y + ratia2);
            ratia2 += 0.0001; //0.005
         }
      }
      ratia2 += 0.0003; //0.005
   }
   glEnd();
}

//cicloida
void Display6(){
   // t \in R
   
   double pi = 4 * atan(1);
   double ratia = 0.05;

   double a = 0.1;
   double b = 0.2;

   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP); 
   for(double t = -4*pi; t <= 4*pi; t+=0.05){
      double x, y;

      x = a * t - b * sin(t);
      y = a - b * cos(t);
      glVertex2d(x,y);
   }
   glEnd();
}

//epicicloida
void Display7(){
   //
   double pi = 4 * atan(1);
   double ratia = 0.05;

   double R = 0.1;
   double r = 0.3;

   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP); 
   for(double t = 0; t <= 2*pi; t+=0.05){
      double x, y;

      x = (R + r) * cos( (r/R) * t) - r * cos(t + (r/R) * t );
      y = (R + r) * sin( (r/R) * t) - r * sin(t + (r/R) * t );
      glVertex2d(x,y);
   }
   glEnd();
}

//hipocicloida
void Display8(){
   //
   double pi = 4 * atan(1);
   double ratia = 0.05;

   double R = 0.1;
   double r = 0.3;

   glColor3f(1,0.1,0.1); // rosu
   glBegin(GL_LINE_STRIP); 
   for(double t = 0; t <= 2*pi; t+=0.05){
      double x, y;
      x = (R - r) * cos( (r/R) * t) - r * cos(t - (r/R) * t );
      y = (R - r) * sin( (r/R) * t) - r * sin(t - (r/R) * t );

      glVertex2d(x,y);
   }
   glEnd();
}

//lemniscata lui Bernoulli
void Display9(){
   double pi = 4 * atan(1.0);
   double a,x,y,t,r;
   double ratia = 0.005;
   a = 0.4;


   glColor3f(1,0.1,0.1);
   glBegin(GL_LINE_STRIP);

   for (t = pi/4 - ratia; t >-pi/4; t -= ratia){
      r = -a * (sqrt( 2 * cos( 2 * t ) ) );
      x = r * cos(t);
      y = r * sin(t);
      glVertex2f(x,y);
   }
   for (t = -pi/4 +ratia; t < pi/4; t += ratia){
      r = a * (sqrt( 2 * cos( 2 * t ) ) );
      x = r * cos(t);
      y = r * sin(t);
      glVertex2f(x,y);
   }
   glEnd();
}

//spirala logaritmica
void Display10(){
   double pi = 4 * atan(1.0);
   double a,x,y,t,r;
   double e = 2.71828;
   double ratia = 0.05;
   a = 0.02;

   glColor3f(1,0.1,0.1);
   glBegin(GL_LINE_STRIP);
 
  for (t = 0; t < 100; t += ratia){
      r = a * pow(e,1+t);
      x = r * cos(t);
      y = r * sin(t);
      glVertex2f(x,y);
   }
   glEnd();
}


void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);

   glLineWidth(1);

//   glPointSize(4);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
   glClear(GL_COLOR_BUFFER_BIT);

   switch(prevKey) {
   case '1':
      Display1();
      break;
   case '2':
      Display2();
      break;
   case '3':
      Display3();
      break;
   case '4':
      Display4();
      break;
   case '5':
      Display5();
      break;
   case '6':
      Display6();
      break;
   case '7':
      Display7();
      break;
   case '8':
      Display8();
      break;
   case '9':
      Display9();
      break;
   case '0':
      Display10();
      break;
   default:
      break;
   }

   glFlush();
}

void Reshape(int w, int h) {
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
   prevKey = key;
   if (key == 27) // escape
      exit(0);
   glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

   glutInit(&argc, argv);
   
   glutInitWindowSize(dim, dim);
   glutInitWindowPosition(100, 100);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   glutCreateWindow (argv[0]);

   Init();

   glutReshapeFunc(Reshape);
   
   glutKeyboardFunc(KeyboardFunc);
   glutMouseFunc(MouseFunc);
   glutDisplayFunc(Display);
   
   glutMainLoop();

   return 0;
}