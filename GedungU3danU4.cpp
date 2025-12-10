#define _USE_MATH_DEFINES
#include <GL/glut.h>
#include <cmath>
#include <stdio.h>

// ==================== KONSTANTA MODEL ====================
const float BUILDING_WIDTH = 12.0f;
const float BUILDING_HEIGHT = 20.0f;
const float BUILDING_DEPTH = 16.0f;
const float GAP_SIZE = 6.0f;
const float FLOOR_HEIGHT = 4.0f; // Tinggi per lantai

// ==================== VARIABEL KAMERA ====================
float cameraAngleX = 20.0f;    // Sudut vertikal
float cameraAngleY = -35.0f;   // Sudut horizontal
float cameraDistance = 50.0f;  // Jarak kamera
float cameraHeight = 5.0f;     // Tinggi kamera

// ==================== VARIABEL MOUSE ====================
int lastMouseX = 0;
int lastMouseY = 0;
bool isMouseDragging = false;

// ==================== VARIABEL ROTASI & SKALA ====================
float buildingRotationY = 0.0f;  // Rotasi gedung
float buildingScale = 1.0f;      // Skala gedung

// ==================== WARNA ====================
GLfloat COLOR_WHITE_PANEL[] = { 0.92f, 0.92f, 0.92f };
GLfloat COLOR_DARK_GREY_PANEL[] = { 0.65f, 0.65f, 0.65f };
//GLfloat COLOR_WINDOW[] = { 0.15f, 0.15f, 0.2f };
GLfloat COLOR_BRIDGE_FRAME[] = { 0.5f, 0.5f, 0.5f };
GLfloat COLOR_GROUND[] = { 0.45f, 0.45f, 0.45f };
GLfloat COLOR_COLUMN[] = { 0.8f, 0.8f, 0.8f };

// ==================== FUNGSI HELPER ====================
void setColor(float r, float g, float b) {
    glColor3f(r, g, b);
    GLfloat mat_color[] = { r, g, b, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color);
}

void drawBox(float w, float h, float d) {
    glPushMatrix();
    glScalef(w, h, d);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// ==================== MENGGAMBAR PANEL FACADE ====================
void drawFacadePanel(float x, float y, float w, float h, bool isWindow) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    if (isWindow) {
        setColor(0.15f, 0.15f, 0.2f); // Window color
        drawBox(w * 0.85f, h * 0.85f, 0.05f);
    }
    else {
        setColor(0.92f, 0.92f, 0.92f); // White panel
        drawBox(w, h, 0.05f);
    }
    glPopMatrix();
}

// ==================== MENGGAMBAR FASAD LENGKAP ====================
void drawBuildingFacade(float width, float height, float depth, int numFloors) {
    float floorH = height / numFloors;
    int panelsPerFloor = 6;
    float panelW = width / panelsPerFloor;

    // Fasad Depan
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, depth / 2.0f + 0.05f);

    for (int floor = 0; floor < numFloors; floor++) {
        float floorY = height / 2.0f - floor * floorH - floorH / 2.0f;

        for (int panel = 0; panel < panelsPerFloor; panel++) {
            float panelX = -width / 2.0f + panel * panelW + panelW / 2.0f;

            // Panel horizontal band (strip putih)
            drawFacadePanel(panelX, floorY - floorH / 2.0f + 0.2f, panelW, 0.4f, false);

            // Jendela
            if (floor > 0) { // Skip ground floor
                drawFacadePanel(panelX, floorY, panelW, floorH * 0.6f, true);
            }
        }
    }
    glPopMatrix();

    // Fasad Belakang
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -depth / 2.0f - 0.05f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

    for (int floor = 0; floor < numFloors; floor++) {
        float floorY = height / 2.0f - floor * floorH - floorH / 2.0f;

        for (int panel = 0; panel < panelsPerFloor; panel++) {
            float panelX = -width / 2.0f + panel * panelW + panelW / 2.0f;
            drawFacadePanel(panelX, floorY - floorH / 2.0f + 0.2f, panelW, 0.4f, false);

            if (floor > 0) {
                drawFacadePanel(panelX, floorY, panelW, floorH * 0.6f, true);
            }
        }
    }
    glPopMatrix();

    // Sisi Kiri - Abu-abu gelap dengan jendela
    glPushMatrix();
    glTranslatef(-width / 2.0f - 0.05f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    setColor(0.65f, 0.65f, 0.65f);
    drawBox(depth, height, 0.1f);
    glPopMatrix();

    // Jendela sisi kiri
    glPushMatrix();
    glTranslatef(-width / 2.0f - 0.15f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    for (int floor = 1; floor < numFloors; floor++) {
        float floorY = height / 2.0f - floor * floorH - floorH / 2.0f;

        for (int win = 0; win < 3; win++) {
            float winZ = -depth / 3.0f + win * (depth / 3.0f);
            glPushMatrix();
            glTranslatef(winZ, floorY, 0.02f);
            setColor(0.15f, 0.15f, 0.2f);
            drawBox(depth / 4.0f, floorH * 0.6f, 0.05f);
            glPopMatrix();
        }
    }
    glPopMatrix();

    // Sisi Kanan - Abu-abu gelap dengan jendela
    glPushMatrix();
    glTranslatef(width / 2.0f + 0.05f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    setColor(0.65f, 0.65f, 0.65f);
    drawBox(depth, height, 0.1f);
    glPopMatrix();

    // Jendela sisi kanan
    glPushMatrix();
    glTranslatef(width / 2.0f + 0.15f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);

    for (int floor = 1; floor < numFloors; floor++) {
        float floorY = height / 2.0f - floor * floorH - floorH / 2.0f;

        for (int win = 0; win < 3; win++) {
            float winZ = -depth / 3.0f + win * (depth / 3.0f);
            glPushMatrix();
            glTranslatef(winZ, floorY, 0.02f);
            setColor(0.15f, 0.15f, 0.2f);
            drawBox(depth / 4.0f, floorH * 0.6f, 0.05f);
            glPopMatrix();
        }
    }
    glPopMatrix();
}

// ==================== MENGGAMBAR KOLOM LANTAI DASAR ====================
void drawGroundColumns(float width, float depth) {
    float columnSize = 0.8f;
    float columnHeight = FLOOR_HEIGHT * 0.9f;
    float baseY = -BUILDING_HEIGHT / 2.0f + columnHeight / 2.0f;

    setColor(0.8f, 0.8f, 0.8f); // Column color

    // 6 kolom di depan
    for (int i = 0; i < 6; i++) {
        float x = -width / 2.0f + (i + 0.5f) * (width / 6.0f);
        glPushMatrix();
        glTranslatef(x, baseY, depth / 2.0f - 1.0f);
        drawBox(columnSize, columnHeight, columnSize);
        glPopMatrix();
    }

    // 6 kolom di belakang
    for (int i = 0; i < 6; i++) {
        float x = -width / 2.0f + (i + 0.5f) * (width / 6.0f);
        glPushMatrix();
        glTranslatef(x, baseY, -depth / 2.0f + 1.0f);
        drawBox(columnSize, columnHeight, columnSize);
        glPopMatrix();
    }
}

// ==================== MENGGAMBAR GEDUNG LENGKAP ====================
void drawCompleteBuilding() {
    int numFloors = 5;

    // Core structure
    setColor(0.65f, 0.65f, 0.65f);
    drawBox(BUILDING_WIDTH, BUILDING_HEIGHT, BUILDING_DEPTH);

    // Facade dengan panel dan jendela
    drawBuildingFacade(BUILDING_WIDTH, BUILDING_HEIGHT, BUILDING_DEPTH, numFloors);

    // Kolom ground floor
    drawGroundColumns(BUILDING_WIDTH, BUILDING_DEPTH);

    // Atap
    glPushMatrix();
    glTranslatef(0.0f, BUILDING_HEIGHT / 2.0f + 0.15f, 0.0f);
    setColor(0.65f, 0.65f, 0.65f);
    drawBox(BUILDING_WIDTH + 0.2f, 0.3f, BUILDING_DEPTH + 0.2f);
    glPopMatrix();
}

// ==================== MENGGAMBAR SKYBRIDGE ====================
void drawSkybridge(float yPos, float bridgeWidth) {
    float bridgeHeight = FLOOR_HEIGHT * 0.8f;
    float bridgeDepth = BUILDING_DEPTH * 0.7f;

    // Frame utama
    glPushMatrix();
    glTranslatef(0.0f, yPos, 0.0f);
    setColor(0.5f, 0.5f, 0.5f);
    drawBox(bridgeWidth, bridgeHeight, bridgeDepth);
    glPopMatrix();

    // Glass panels - Depan (tanpa blending dulu)
    glPushMatrix();
    glTranslatef(0.0f, yPos, bridgeDepth / 2.0f + 0.05f);
    setColor(0.15f, 0.15f, 0.2f);
    drawBox(bridgeWidth * 0.9f, bridgeHeight * 0.8f, 0.05f);
    glPopMatrix();

    // Glass panels - Belakang
    glPushMatrix();
    glTranslatef(0.0f, yPos, -bridgeDepth / 2.0f - 0.05f);
    setColor(0.15f, 0.15f, 0.2f);
    drawBox(bridgeWidth * 0.9f, bridgeHeight * 0.8f, 0.05f);
    glPopMatrix();

    // Horizontal louvers/fins (Detail arsitektur)
    int numLouvers = 8;
    for (int i = 0; i < numLouvers; i++) {
        float louverY = yPos + bridgeHeight / 2.0f - (i * bridgeHeight / numLouvers);

        glPushMatrix();
        glTranslatef(0.0f, louverY, bridgeDepth / 2.0f + 0.1f);
        setColor(0.5f, 0.5f, 0.5f);
        drawBox(bridgeWidth, 0.15f, 0.05f);
        glPopMatrix();
    }
}

// ==================== MENGGAMBAR SELURUH KOMPLEKS ====================
void drawEntireComplex() {
    // Gedung Kiri
    glPushMatrix();
    glTranslatef(-(BUILDING_WIDTH / 2.0f + GAP_SIZE / 2.0f), 0.0f, 0.0f);
    drawCompleteBuilding();
    glPopMatrix();

    // Gedung Kanan
    glPushMatrix();
    glTranslatef(BUILDING_WIDTH / 2.0f + GAP_SIZE / 2.0f, 0.0f, 0.0f);
    drawCompleteBuilding();
    glPopMatrix();

    // 3 Skybridge bertingkat (Posisi disesuaikan, yang atas diturunkan)
    float bridgeWidth = GAP_SIZE + 1.0f;
    drawSkybridge(BUILDING_HEIGHT * 0.10f, bridgeWidth);  // Bridge bawah
    drawSkybridge(BUILDING_HEIGHT * 0.28f, bridgeWidth);  // Bridge tengah
    drawSkybridge(BUILDING_HEIGHT * 0.42f, bridgeWidth);  // Bridge atas (diturunkan)
}

// ==================== DISPLAY FUNCTION ====================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Setup kamera dengan glTranslate
    float camY = cameraDistance * sin(cameraAngleX * M_PI / 180.0f);
    float camXZ = cameraDistance * cos(cameraAngleX * M_PI / 180.0f);
    float camX = camXZ * sin(cameraAngleY * M_PI / 180.0f);
    float camZ = camXZ * cos(cameraAngleY * M_PI / 180.0f);

    gluLookAt(camX, camY + cameraHeight, camZ,
        0.0f, cameraHeight, 0.0f,
        0.0f, 1.0f, 0.0f);

    // Apply transformasi: glRotate, glScale
    glRotatef(buildingRotationY, 0.0f, 1.0f, 0.0f);
    glScalef(buildingScale, buildingScale, buildingScale);

    // Gambar kompleks gedung
    drawEntireComplex();

    // Ground/Lantai
    glPushMatrix();
    glTranslatef(0.0f, -BUILDING_HEIGHT / 2.0f - 0.1f, 0.0f);
    setColor(0.45f, 0.45f, 0.45f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    float groundSize = 80.0f;
    glVertex3f(-groundSize, 0.0f, groundSize);
    glVertex3f(groundSize, 0.0f, groundSize);
    glVertex3f(groundSize, 0.0f, -groundSize);
    glVertex3f(-groundSize, 0.0f, -groundSize);
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

// ==================== INIT ====================
void init() {
    glClearColor(0.7f, 0.75f, 0.85f, 1.0f); // Sky blue
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    // Lighting (BONUS)
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat light_pos[] = { 20.0f, 30.0f, 20.0f, 1.0f };
    GLfloat light_ambient[] = { 0.3f, 0.3f, 0.35f, 1.0f };
    GLfloat light_diffuse[] = { 0.9f, 0.9f, 0.85f, 1.0f };
    GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // Shading (BONUS)
    glShadeModel(GL_SMOOTH);

    // Material properties
    GLfloat mat_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat mat_shininess[] = { 30.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

// ==================== RESHAPE ====================
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, (GLfloat)w / (GLfloat)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

// ==================== KEYBOARD (Interaksi) ====================
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // Zoom
    case 'w': case 'W':
        cameraDistance -= 2.0f;
        if (cameraDistance < 10.0f) cameraDistance = 10.0f;
        break;
    case 's': case 'S':
        cameraDistance += 2.0f;
        break;

        // Rotasi gedung (glRotate)
    case 'a': case 'A':
        buildingRotationY -= 5.0f;
        break;
    case 'd': case 'D':
        buildingRotationY += 5.0f;
        break;

        // Skala gedung (glScale)
    case 'q': case 'Q':
        buildingScale += 0.1f;
        if (buildingScale > 3.0f) buildingScale = 3.0f;
        break;
    case 'e': case 'E':
        buildingScale -= 0.1f;
        if (buildingScale < 0.3f) buildingScale = 0.3f;
        break;

        // Tinggi kamera
    case 'z': case 'Z':
        cameraHeight += 2.0f;
        break;
    case 'x': case 'X':
        cameraHeight -= 2.0f;
        break;

        // Reset
    case 'r': case 'R':
        cameraAngleX = 20.0f;
        cameraAngleY = -35.0f;
        cameraDistance = 50.0f;
        cameraHeight = 5.0f;
        buildingRotationY = 0.0f;
        buildingScale = 1.0f;
        break;

    case 27: // ESC
        exit(0);
        break;
    }
    glutPostRedisplay();
}

// ==================== SPECIAL KEYS ====================
void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        cameraAngleX += 3.0f;
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        break;
    case GLUT_KEY_DOWN:
        cameraAngleX -= 3.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
        break;
    case GLUT_KEY_LEFT:
        cameraAngleY += 3.0f;
        break;
    case GLUT_KEY_RIGHT:
        cameraAngleY -= 3.0f;
        break;
    }
    glutPostRedisplay();
}

// ==================== MOUSE (Interaksi) ====================
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isMouseDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else {
            isMouseDragging = false;
        }
    }
}

void motion(int x, int y) {
    if (isMouseDragging) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        cameraAngleY += dx * 0.5f;
        cameraAngleX += dy * 0.3f;

        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

// ==================== MAIN ====================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Campus Building - OpenGL GLUT");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    // Print controls
    printf("=== KONTROL ===\n");
    printf("W/S: Zoom In/Out\n");
    printf("A/D: Rotasi Kiri/Kanan\n");
    printf("Q/E: Scale Besar/Kecil\n");
    printf("Z/X: Naik/Turun Kamera\n");
    printf("Arrow Keys: Putar Kamera\n");
    printf("Mouse Drag: Putar View\n");
    printf("R: Reset\n");
    printf("ESC: Keluar\n");

    glutMainLoop();
    return 0;
}


