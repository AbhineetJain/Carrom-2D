/*
2-D implementation of Carrom using OpenGL2
Rules according to assignment.
*/

#include<iostream>
#include<cmath>
#include<GL/glut.h>
#include<bits/stdc++.h>

using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

//Class declarations
class coin
{
    double rad;
    double mass;
    double x;
    double y;
    double velx;
    double vely;
    double red, blue, green;
    double angle;
    double power;
    int type; //black - 0, white - 1, queen - 2, striker - 3
  
    void checkCollisionWithBoard(double board_size) 
    {
        if(getY() + getRad() + vely > board_size/2 || getY() - getRad() + vely < -board_size/2)
        {
            setVelY(vely*(-0.95));
        }
        if(getX() + getRad() + velx > board_size/2 || getX() - getRad() + velx < -board_size/2)
        {
            setVelX(velx*(-0.95));
        }
    }
    
  public:
    
    void drawCoin()
    {
        glTranslatef(x, y, 0.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        drawCircle(rad);
        glColor3f(red, green, blue);
        drawCircle(rad/1.25);
        glTranslatef(-x, -y, 0.0f);
    }

    coin(double radius = 0.0f, double posx = 0.0f, double posy = 0.0f, int typ = 0, double r = 0.0f, double g = 0.0f, double b = 0.0f, double m = 6)
    {
        red = r; green = g; blue = b;
        rad = radius; 
        x = posx; 
        y = posy;
        type = typ;
        velx = vely = 0.0f;
        mass = m;
        power = 0.05f;
        angle = 90.00f;
    }

    void drawCircle(double rad)
    {
        glBegin(GL_TRIANGLE_FAN);
        for(int i=0 ; i<360 ; i++) {
                glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
        }
        glEnd();
    }

    void init(double posx, double posy, double ang)
    {
        x = posx;
        y = posy;
        velx = vely = 0.0f;
        angle = ang;
        power = 0.05f;
    }

    void setDirectionStriker(double board_size, double ang = 0.0f, double pwr = 0.0f)
    {
        angle += ang;
        power += pwr;
        power = max(power, 0.05);
        power = min(power, 1.0);
        drawPowerDir(board_size);
    }

    void setMouseDirectionStriker(double board_size, double ang, double pwr)
    {
        angle = ang;
        power = pwr;
        power = max(power, 0.05);
        power = min(power, 1.0);
        drawPowerDir(board_size);
    }

    void drawPowerDir(double board_size)
    {
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(x, y);
        glVertex2f(x+4.5*cos(DEG2RAD(angle)), y+(4.5*sin(DEG2RAD(angle))));
        glEnd();
        glTranslatef(1.5*board_size, board_size/2, -8.0f);
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(-0.2f, power*5 - 5);
        glVertex2f(0.5f, power*5 - 5);
        glEnd();
        glTranslatef(-1.5*board_size, -board_size/2, 8.0f);
    }

    void set_vel()
    {
        velx = power*cos(DEG2RAD(angle))/3;
        vely = power*sin(DEG2RAD(angle))/3;
    }

    void hit(double board_size, double friction)
    {
        checkCollisionWithBoard(board_size);
        x += velx;
        y += vely;
        velx = velx*friction;
        vely = vely*friction;
    }

    void increase_x_y(double dx, double dy, int turn, double board_size)
    {
        if(turn == 1 && (x+dx >= board_size/2.825f-0.0002 || x+dx <= -board_size/2.825f+0.0002))
            dx = dy = 0;
        x += dx;
        y += dy;
    }

    void printVars()
    {
        cout << x << " " << y << " " << rad << endl;
    }

    double getX() { return x; }
    double getY() { return y; }
    double getVelX() { return velx; }
    double getVelY() { return vely; }
    double getRad() { return rad; }
    double getMass() { return mass; }
    void printColor() { cout << red << " " << green << " " << blue << endl; }
    int getType() { return type; }
    void setX(double posx) { x = posx; }
    void setY(double posy) { y = posy; }
    void setVelX(double vx) { velx = vx; }
    void setVelY(double vy) { vely = vy; }
};

class game
{
    double board_size;
    double pocket_rad;
    double base_len;
    double centre_rad;
    double board_r, board_g, board_b;
    
  public:
    
    void drawBoard()
    {
        glTranslatef(0.0f, 0.0f, -8.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3f(board_r, board_g, board_b);
	    glRectf(-board_size/2, board_size/2, board_size/2, -board_size/2);
        
        //Base Lines Begin - Top
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(-board_size/2.825f, board_size/2.75f);
        glVertex2f(board_size/2.825f, board_size/2.75f);
        glVertex2f(-board_size/2.825f, board_size/2.5f);
        glVertex2f(board_size/2.825f, board_size/2.5f);
        glEnd();
        glTranslatef(-board_size/2.825, board_size/2.625, 0.0f);
        drawCircle(board_size/55.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawCircle(board_size/75.0f);
        glTranslatef(2*board_size/2.825, 0.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        drawCircle(board_size/55.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawCircle(board_size/75.0f);
        glTranslatef(-board_size/2.825, -board_size/2.625, 0.0f);
        
        //Base Line - Bottom
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(board_size/2.825f, -board_size/2.75f);
        glVertex2f(-board_size/2.825f, -board_size/2.75f);
        glVertex2f(board_size/2.825f, -board_size/2.5f);
        glVertex2f(-board_size/2.825f, -board_size/2.5f);
        glEnd();
        glTranslatef(-board_size/2.825, -board_size/2.625, 0.0f);
        drawCircle(board_size/55.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawCircle(board_size/75.0f);
        glTranslatef(2*board_size/2.825, 0.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        drawCircle(board_size/55.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawCircle(board_size/75.0f);
        glTranslatef(-board_size/2.825, board_size/2.625, 0.0f);
        
        //Base Line - Right
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(board_size/2.75f, board_size/2.825f);
        glVertex2f(board_size/2.75f, -board_size/2.825f);
        glVertex2f(board_size/2.5f, board_size/2.825f);
        glVertex2f(board_size/2.5f, -board_size/2.825f);
        glEnd();
        glTranslatef(board_size/2.625, board_size/2.825, 0.0f);
        drawCircle(board_size/55.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawCircle(board_size/75.0f);
        glTranslatef(0.0f, -2*board_size/2.825, 0.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        drawCircle(board_size/55.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawCircle(board_size/75.0f);
        glTranslatef(-board_size/2.625, board_size/2.825, 0.0f);
        
        //Base Line - Left
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(-board_size/2.75f, board_size/2.825f);
        glVertex2f(-board_size/2.75f, -board_size/2.825f);
        glVertex2f(-board_size/2.5f, board_size/2.825f);
        glVertex2f(-board_size/2.5f, -board_size/2.825f);
        glEnd();
        glTranslatef(-board_size/2.625, board_size/2.825, 0.0f);
        drawCircle(board_size/55.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawCircle(board_size/75.0f);
        glTranslatef(0.0f, -2*board_size/2.825, 0.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        drawCircle(board_size/55.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawCircle(board_size/75.0f);
        glTranslatef(board_size/2.625, board_size/2.825, 0.0f);
    }
    
    void drawBoundary()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3f(0.0f, 0.0f, 0.0f);
        glTranslatef(-board_size/2, 0.0f, 0.0f);
        glRectf(-board_size/24, board_size/2, 0.0f, -board_size/2);
        glRectf(-board_size/24, 13*board_size/24, board_size, board_size/2);
        glRectf(-board_size/24, -board_size/2, board_size, -13*board_size/24);
        glRectf(board_size, 13*board_size/24, 25*board_size/24, -13*board_size/24);
        glTranslatef(board_size/2, 0.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        drawCircleOutline(board_size/10);
        drawCircleOutline(board_size/9);
        drawCircle(board_size/50.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawCircle(board_size/60.0f);

        //Draw Power Gradient
        glTranslatef(1.5*board_size, board_size/2, -8.0f);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.3f, 0.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(0.3f, -2.5f);
        glVertex2f(0.0f, -2.5f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(0.0f, -2.5f);
        glVertex2f(0.3f, -2.5f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(0.3f, -5.0f);
        glVertex2f(0.0f, -5.0f);
        glEnd();
        glTranslatef(-1.5*board_size, -board_size/2, 8.0f);

    }
  
    void drawPockets()
    {
        double center_x = 0, center_y = 0;
        glTranslatef(-17*board_size/18, 17*board_size/18, -8.0f);
        for(int i=0; i<4; i++)
        {
            glTranslatef(center_x, center_y, 0.0f);
            center_x = center_y = 0;
            glColor3f(0.0f, 0.0f, 0.0f);
            drawCircle(board_size/16);
            glColor3f(0.6f, 0.5f, 0.0f);
            drawCircle(board_size/18);
            if(i==0) center_x += 17*board_size/9;
            else if(i==1) center_y -= 17*board_size/9;
            else if(i==2) center_x -= 17*board_size/9;
        }
    }

    void fillPocketGaps()
    {
        double center_x = 0, center_y = 0;
        glTranslatef(-17*board_size/17.5, 17*board_size/17.5, -8.0f);
        for(int i=0; i<4; i++)
        {
            glTranslatef(center_x, center_y, 0.0f);
            center_x = center_y = 0;
            glColor3f(0.0f, 0.0f, 0.0f);
            drawCircle(board_size/22);
            if(i==0) center_x += 17*board_size/8.75;
            else if(i==1) center_y -= 17*board_size/8.75;
            else if(i==2) center_x -= 17*board_size/8.75;
        }
    }
  
    void drawCircle(double rad)
    {
        glBegin(GL_TRIANGLE_FAN);
        for(int i=0 ; i<360 ; i+=1) {
                glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
        }
        glEnd();
    }
    
    void drawCircleOutline(double rad)
    {
        glBegin(GL_LINE_LOOP);
        for(int i=0 ; i<360 ; i+=1) {
                glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
        }
        glEnd();
    }
    
    game(double size, double red, double green, double blue)
    {
        board_size = size;
        board_r = red; board_g = green; board_b = blue;
    }
    
};

class player
{
    int whitePotted;
    int blackPotted;
    int queenPotted;
    int score;

  public:

    player()
    {
        whitePotted = blackPotted = queenPotted = 0;
        score = 30;
    }

    void inc_white() { whitePotted++; }
    void inc_black() { blackPotted++; }
    void inc_queen() { queenPotted++; }
    void inc_score(int inc) { score += inc; }
    int ret_score() { return score; }
    int ret_white() { return whitePotted; }
    int ret_black() { return blackPotted; }
    int ret_queen() { return queenPotted; }
};

//Global Variables
double w, h;
double board_size = 5.0f;
game carrom(board_size, 1.0f, 1.0f, 0.8f);

double friction = 0.98f;
double R = 0.6f;

int reset = 1, starttime = 1;
int gameOver = 0;

int whitePotted = 0;
int blackPotted = 0;
int queenPotted = 0;

int turn = 0;

time_t t_begin, t_end, prev_t_end;

char scoreString1[15];
char scoreString2[15];
char gameOverString[20] = "Game Over!";
char winnerString1[30] = "Winner: Player 1";
char winnerString2[30] = "Winner: Player 2";
char drawString[15] = "Match Drawn.";
char colorString1[15] = "(White)";
char colorString2[15] = "(Black)";

//Player variables
player p1, p2;

//Coin variables
double coin_size = board_size/60.0f;
int no_of_coins = 3;

//Striker variables
double striker_x = 0.0f;
double striker_y = -board_size/2.625f;
double striker_size = board_size/50.0f;

//Coins Array
vector<coin> all_coins(25);
int all_coin_count = 0;

//Function Declarations
void drawScene();
void initRendering();
void handleResize(int w, int h);
void drawPockets(double rad);
void handleKeyPressSpecial(int key, int x, int y);
void handleKeyPress(unsigned char key, int x, int y);
void update(int val);
void checkCollisionWithOtherCoins();
void checkPockets();
void printText(float x, float y, float r, float g, float b, char *string);
void handleMouseClick(int button, int state, int x, int y);
void drawCoinPots();
void drawCoinPottedCoins(int number, double ycoord, double red, double blue, double green);
double translateX(int x);
double translateY(int y);
void handleRight(int x, int y);

int main(int argc, char **argv)
{
    //Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    //Setup the window
    w = glutGet(GLUT_SCREEN_WIDTH);
    h = glutGet(GLUT_SCREEN_HEIGHT);
    glutInitWindowSize(w, h);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Carrom");
    initRendering();
    
    //Initialize coins
    all_coins[all_coin_count++] = coin(striker_size, striker_x, striker_y, 3, 1.0f, 0.5f, 0.0f, 12);
    all_coins[all_coin_count++] = coin(coin_size, 0.0f, 0.0f, 2, 0.984f, 0.058f, 0.75f);
    for(int i=0; i<no_of_coins; i++)
    {
        if(i > 2)
        {
            all_coins[all_coin_count++] = coin(coin_size, 4*(coin_size+0.01)*cos(DEG2RAD((2*(i+1)-1)*30)), 4*(coin_size+0.01)*sin(DEG2RAD((2*(i+1)-1)*30)), 0);
            all_coins[all_coin_count++] = coin(coin_size, 4*(coin_size+0.01)*cos(DEG2RAD(2*(i+1)*30)), 4*(coin_size+0.01)*sin(DEG2RAD(2*(i+1)*30)), 1, 0.847f, 0.694f, 0.392f);
        }
        else 
        {
            all_coins[all_coin_count++] = coin(coin_size, 2*(coin_size+0.01)*cos(DEG2RAD((2*(i+1)-1)*60)), 2*(coin_size+0.01)*sin(DEG2RAD((2*(i+1)-1)*60)), 0);
            all_coins[all_coin_count++] = coin(coin_size, 2*(coin_size+0.01)*cos(DEG2RAD(2*(i+1)*60)), 2*(coin_size+0.01)*sin(DEG2RAD(2*(i+1)*60)), 1, 0.847f, 0.694f, 0.392f);
        }
    }
    // Register callbacks
    glutDisplayFunc(drawScene);
    glutIdleFunc(drawScene);
    glutKeyboardFunc(handleKeyPress);
    glutSpecialFunc(handleKeyPressSpecial);
    glutMouseFunc(handleMouseClick);
    glutReshapeFunc(handleResize);
    glutTimerFunc(10, update, 0);

    glutMainLoop();
    return 0;
    
}

// Function to draw objects on the screen
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    
    //Draw Board
    carrom.drawBoard();
    carrom.drawBoundary();
    
    glPushMatrix();
    carrom.fillPocketGaps();
    glPopMatrix();

    glPushMatrix();
    carrom.drawPockets();
    glPopMatrix();
    
    glPushMatrix();
    for(int i=0; i<all_coin_count; i++)
        all_coins[i].drawCoin();
    if(reset)
    {
        all_coins[0].setDirectionStriker(board_size);
        if(starttime == 1 && gameOver == 0)
        {
            t_begin = time(0);
            prev_t_end = t_begin;
            starttime = 0;
        }
    }
    sprintf(scoreString1, "Player 1: %d", p1.ret_score());
    sprintf(scoreString2, "Player 2: %d", p2.ret_score());
    printText(-board_size, -board_size/2, 0.0f, 0.0f, 0.0f, scoreString1);
    printText(-board_size, -board_size/2.2, 0.0f, 0.0f, 0.0f, colorString1);
    printText(-board_size, board_size/2, 0.0f, 0.0f, 0.0f, scoreString2);
    printText(-board_size, board_size/2.2, 0.0f, 0.0f, 0.0f, colorString2);
    if(gameOver)
    {
        printText(-board_size, 0.0f, 1.0f, 0.0f, 0.0f, gameOverString);
        if(p2.ret_score() > p1.ret_score())
            printText(-board_size, board_size/4, 1.0f, 0.0f, 0.0f, winnerString2);
        else if(p2.ret_score() < p1.ret_score())
            printText(-board_size, -board_size/4, 1.0f, 0.0f, 0.0f, winnerString1);
        else
            printText(-board_size, -board_size/4, 1.0f, 0.0f, 0.0f, drawString);
    }
    drawCoinPots();
    glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();
        
}

// Initializing some openGL 3D rendering options
void initRendering() {

	glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
	glEnable(GL_COLOR_MATERIAL);    // Enable coloring
	glClearColor(255.0f, 255.0f, 255.0f, 1.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(45.0f, (double)w / (double)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void handleKeyPress(unsigned char key, int x, int y) {

    if (key == 27) {
        exit(0);     // escape key is pressed
    }
    if (key == 65 || key == 97) {   // a is pressed
        if(reset)
            all_coins[0].setDirectionStriker(board_size, 1.0f, 0.0f);        
    }
    if (key == 67 || key == 99) { // c is pressed
        if(reset)
            all_coins[0].setDirectionStriker(board_size, -1.0f, 0.0f); 
    }
    if (key == 32) { // space is pressed
        if(reset)
        {
            reset = 0;
            all_coins[0].set_vel();
        }
    }    
}

void handleKeyPressSpecial(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
        if(reset)
            all_coins[0].increase_x_y(-0.05, 0, 1, board_size); //move striker on base line to left
    if (key == GLUT_KEY_RIGHT)
        if(reset)
            all_coins[0].increase_x_y(0.05, 0, 1, board_size);  //move striker on base line to right
    if (key == GLUT_KEY_UP)
        if(reset)
            all_coins[0].setDirectionStriker(board_size, 0.0f, 0.05f);  //increase initial velocity/power of striker
    if (key == GLUT_KEY_DOWN)
        if(reset)
            all_coins[0].setDirectionStriker(board_size, 0.0f, -0.05f); //decrease initial velocity/power of striker
}

void update(int val)
{
    checkCollisionWithOtherCoins(); //Check inter-coin collision

    for(int i=0; i<all_coin_count; i++) /*Propagate all the available coins on board*/
    {
        all_coins[i].hit(board_size, friction);
    }

    checkPockets(); //Check if any coin is pocketed
    
    if(gameOver == 0 && starttime == 0 && reset)    /*Reduce score every second according to turn*/
    {
        t_end = time(0);
        if(t_end > prev_t_end)
        {
            if(turn == 0)
                p1.inc_score(-1);
            else
                p2.inc_score(-1);
        }
        prev_t_end = t_end;
    }

    for(int i=0, j=0; i<all_coin_count; i++)    //Checks if all coins have stopped moving
    {
        if(fabs(all_coins[i].getVelX()) < 1e-4 && fabs(all_coins[i].getVelY()) < 1e-4 && reset == 0 && gameOver == 0)
        {
            j++;
        }
        if(j == all_coin_count) //Changes the turn if all have stopped.
        {
            reset = 1;
            starttime = 1;
            turn = 1 - turn;
            if(turn == 0)
                all_coins[0].init(striker_x, striker_y, 90.0f);
            else
                all_coins[0].init(striker_x, -striker_y, 270.0f);
        }
    }

    //Game Over conditions checked.
    if(whitePotted == no_of_coins && queenPotted == 1)
    {
        gameOver = 1;       
    }
    else if(blackPotted == no_of_coins && queenPotted == 1)
    {
        gameOver = 1;
    }

    glutTimerFunc(10, update, 0);
}

void checkPockets()
{
    for(int i=0; i<all_coin_count; i++)
    {
        //Checks distance of all coins from pockets and takes different actions for different types of coins, including striker (type 3)
        if(sqrt(pow(17*board_size/36 - all_coins[i].getX(), 2)+pow(17*board_size/36 - all_coins[i].getY(), 2)) <= board_size/36
            || sqrt(pow(-17*board_size/36 - all_coins[i].getX(), 2)+pow(17*board_size/36 - all_coins[i].getY(), 2)) <= board_size/36
            || sqrt(pow(17*board_size/36 - all_coins[i].getX(), 2)+pow(-17*board_size/36 - all_coins[i].getY(), 2)) <= board_size/36
            || sqrt(pow(-17*board_size/36 - all_coins[i].getX(), 2)+pow(-17*board_size/36 - all_coins[i].getY(), 2)) <= board_size/36)
        {
            if(all_coins[i].getType() != 3)
            {
                if(all_coins[i].getType() == 0)
                {
                    blackPotted++;
                    if(turn == 0)
                    {
                        p1.inc_score(-5);
                        p1.inc_black();
                    }
                    else
                    {
                        p2.inc_score(10);
                        p2.inc_black();
                    }
                }    
                else if(all_coins[i].getType() == 1)
                {
                    whitePotted++;
                    if(turn == 0)
                    {
                        p1.inc_score(10);
                        p1.inc_white();
                    }
                    else
                    {
                        p2.inc_score(-5);
                        p2.inc_white();
                    }
                }
                else if(all_coins[i].getType() == 2)
                {
                    queenPotted++;
                    if(turn == 0)
                    {
                        p1.inc_score(50);
                        p1.inc_queen();
                    }
                    else
                    {
                        p2.inc_score(50);
                        p2.inc_queen();
                    }
                }
                all_coins.erase(all_coins.begin()+i);  //Deletes the coin from the board, and the array
                i--;
                all_coin_count--;
            }
            else
            {
                reset = 1;  //Foul on pocketing the striker, the turn changes instantly
                if(turn == 0)
                    p1.inc_score(-10);
                else
                    p2.inc_score(-10);
                turn = 1 - turn;
                if(turn == 0)
                    all_coins[0].init(striker_x, striker_y, 90.0f);
                else
                    all_coins[0].init(striker_x, -striker_y, 270.0f);      
            }
        }
        
    }
}

void checkCollisionWithOtherCoins()
{
    double slope, dv, mass_ratio, rel_x, rel_y, rel_vel_x, rel_vel_y, vel_x_cm, vel_y_cm;
    int flag = 0;
    for(int i=0; i<all_coin_count; i++)
    {
        for(int j=i+1; j<all_coin_count; j++)
        {
            //Checks for conditions of collision between any two coins
            if(sqrt(pow(all_coins[i].getX() - all_coins[j].getX(), 2) + pow(all_coins[i].getY() - all_coins[j].getY(), 2)) 
                > all_coins[i].getRad() + all_coins[j].getRad())
            {
                if(sqrt(pow(all_coins[i].getX() + all_coins[i].getVelX() - all_coins[j].getX() - all_coins[j].getVelX(), 2) + 
                    pow(all_coins[i].getY() + all_coins[i].getVelY() - all_coins[j].getY() - all_coins[j].getVelY(), 2))
                    <= all_coins[i].getRad() + all_coins[j].getRad())
                {
                    
                    flag = 1;
                }
            }
            else 
            {
                flag = 1;        
            }

            //If condition for collision is true
            if(flag == 1)
            {
                mass_ratio = all_coins[j].getMass()/all_coins[i].getMass();
                rel_x = all_coins[j].getX()-all_coins[i].getX();
                rel_y = all_coins[j].getY()-all_coins[i].getY();
                rel_vel_x = all_coins[j].getVelX()-all_coins[i].getVelX();
                rel_vel_y = all_coins[j].getVelY()-all_coins[i].getVelY();
                vel_x_cm = (all_coins[i].getMass()*all_coins[i].getVelX() + all_coins[j].getMass()*all_coins[j].getVelX())/(all_coins[i].getMass() + all_coins[j].getMass());
                vel_y_cm = (all_coins[i].getMass()*all_coins[i].getVelY() + all_coins[j].getMass()*all_coins[j].getVelY())/(all_coins[i].getMass() + all_coins[j].getMass());

                slope = rel_y/rel_x;
                dv = -2 * (rel_vel_x + slope*rel_vel_y) / ((1 + mass_ratio) * (1 + slope*slope));
                all_coins[j].setVelX(all_coins[j].getVelX() + dv);
                all_coins[j].setVelY(all_coins[j].getVelY() + slope*dv);
                all_coins[i].setVelX(all_coins[i].getVelX() - dv*mass_ratio);
                all_coins[i].setVelY(all_coins[i].getVelY() - slope*dv*mass_ratio);

                //Update inelastic collision velocities
                all_coins[i].setVelX((all_coins[i].getVelX() - vel_x_cm)*R + vel_x_cm);
                all_coins[i].setVelY((all_coins[i].getVelY() - vel_y_cm)*R + vel_y_cm);
                all_coins[j].setVelX((all_coins[j].getVelX() - vel_x_cm)*R + vel_x_cm);
                all_coins[j].setVelY((all_coins[j].getVelY() - vel_y_cm)*R + vel_y_cm);
                flag = 0;
            }
        }
    }
}


//Function to print any given string of type char *
void printText(float x, float y, float r, float g, float b, char *string)
{
  glColor3f( r, g, b );
  glRasterPos2f(x, y);
  int len, i;
  len = (int)strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
  }
}

//Handler for mouse click event
void handleMouseClick(int button, int state, int x, int y)
{
    double posx = translateX(x);
    double posy = translateY(y);
    if(button == GLUT_RIGHT_BUTTON)
        glutMotionFunc(handleRight);    //Calls the drag/motion mouse handler
    else if(button == GLUT_LEFT_BUTTON)
    {
        glutMotionFunc(NULL); //The handler needs to be negated 
        if(state == GLUT_DOWN)  //Picks up the coordinates in term of board, and calculates angle and power.
        {
            if(posx <= board_size/2 && posx >= -board_size/2 && posy <= board_size/2 && posy >= -board_size/2 && reset)
            {
                double rely = (posy - all_coins[0].getY());
                double relx = (posx - all_coins[0].getX());
                double ang = (atan2(rely, relx) * 180 / PI);
                double pwr = sqrt(rely*rely + relx*relx)/board_size;
                all_coins[0].setMouseDirectionStriker(board_size, ang, pwr);
            }
        }
        else if(state == GLUT_UP)   //Similar to handling spacebar event, initiates striker
        {
            if(reset)
            {
                reset = 0;
                all_coins[0].set_vel();
            }
        }
    }
}

//Handler for right mouse button drag event
void handleRight(int x, int y)
{
    double posx = translateX(x);
    double posy = translateY(y);
    double strikerx = all_coins[0].getX();
    double strikersize = all_coins[0].getRad();
    //Check for position of the cursor, to be inside the striker
    if((posy <= -board_size/2.75 && posy >= -board_size/2.5) || (posy >= 1.98763 && posy <= 2.18177))
        if(posx <= strikerx + strikersize && posx >= strikerx - strikersize && posx <= board_size/2.825f-0.0002 && posx >= -board_size/2.825f+0.0002)
        all_coins[0].setX(posx);
}

//Calls to draw coins for various different combinations
void drawCoinPots()
{
    drawCoinPottedCoins(p1.ret_white(), -board_size/3.2, 0.847f, 0.694f, 0.392f);
    drawCoinPottedCoins(p1.ret_black(), -board_size/3.2 - 2.25*coin_size, 0.0f, 0.0f, 0.0f);
    drawCoinPottedCoins(p1.ret_queen(), -board_size/3.2 - 4.5*coin_size, 0.984f, 0.058f, 0.75f);
    drawCoinPottedCoins(p2.ret_white(), board_size/3, 0.847f, 0.694f, 0.392f);
    drawCoinPottedCoins(p2.ret_black(), board_size/3 + 2.5*coin_size, 0.0f, 0.0f, 0.0f);
    drawCoinPottedCoins(p2.ret_queen(), board_size/3 + 5.0*coin_size, 0.984f, 0.058f, 0.75f);
   
}

//Draws different suites of coins for different players
void drawCoinPottedCoins(int number, double ycoord, double red, double green, double blue)
{
    glTranslatef(-board_size, ycoord, 0.0f);
    for(int i=0; i<number; i++)
    {
        glTranslatef(coin_size*(i*3), 0.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        for(int j=0 ; j<360 ; j++) {
                glVertex2f(coin_size * cos(DEG2RAD(j)), coin_size * sin(DEG2RAD(j)));
        }
        glEnd();
        glColor3f(red, green, blue);
        glBegin(GL_TRIANGLE_FAN);
        for(int j=0 ; j<360 ; j++) {
                glVertex2f(coin_size * cos(DEG2RAD(j)) / 1.25, coin_size * sin(DEG2RAD(j)) / 1.25);
        }
        glEnd();
        glTranslatef(-coin_size*(i*3), 0.0f, 0.0f);
    }
    glTranslatef(board_size, -ycoord, 0.0f);
}

//To normalize the x coordinate provided by mouse events
double translateX(int x)
{
    return ((double)x/w - 0.5)*12.2;
}

//To normalize the y coordinate provided by mouse events
double translateY(int y)
{
    return ((double)y/h - 0.5)*-7.1;
}
