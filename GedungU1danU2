#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>

// ============= KONSTANTA WARNA =============
// Warna Krem Cerah (Untuk Beton/Pemisah Lantai)
#define COLOR_FACADE_KREM 0.96f, 0.93f, 0.88f
// Warna Kaca (Biru Langit Cerah & Transparan)
#define COLOR_GLASS_WINDOW 0.4f, 0.7f, 1.0f, 0.6f
// Warna Plinth (Batu Bawah)
#define COLOR_PLINTH_STONE 0.45f, 0.40f, 0.35f
// Warna Lingkungan
#define COLOR_TREE_LEAVES 0.1f, 0.6f, 0.1f
#define COLOR_TREE_TRUNK 0.4f, 0.3f, 0.2f
#define COLOR_PAVING 0.7f, 0.7f, 0.7f
#define COLOR_ASPHALT 0.2f, 0.2f, 0.2f

// ============= GLOBAL CAMERA =============
float camX = -100.0f;
float camY = 60.0f;
float camZ = -90.0f;
float camYaw = 135.0f;
float camPitch = -25.0f;

// Mouse Interaction
int lastMouseX = 0;
int lastMouseY = 0;
bool isDragging = false;

const float DEG2RAD = 3.14159265f / 180.0f;

// ============= UTILITAS GAMBAR ============
void drawBox(float w, float h, float d) {
    float hw = w * 0.5f, hh = h * 0.5f, hd = d * 0.5f;
    glBegin(GL_QUADS);
    // Depan
    glNormal3f(0, 0, 1); glVertex3f(-hw, -hh, hd); glVertex3f(hw, -hh, hd); glVertex3f(hw, hh, hd); glVertex3f(-hw, hh, hd);
    // Belakang
    glNormal3f(0, 0, -1); glVertex3f(hw, -hh, -hd); glVertex3f(-hw, -hh, -hd); glVertex3f(-hw, hh, -hd); glVertex3f(hw, hh, -hd);
    // Kiri
    glNormal3f(-1, 0, 0); glVertex3f(-hw, -hh, -hd); glVertex3f(-hw, -hh, hd); glVertex3f(-hw, hh, hd); glVertex3f(-hw, hh, -hd);
    // Kanan
    glNormal3f(1, 0, 0); glVertex3f(hw, -hh, hd); glVertex3f(hw, -hh, -hd); glVertex3f(hw, hh, -hd); glVertex3f(hw, hh, hd);
    // Atas
    glNormal3f(0, 1, 0); glVertex3f(-hw, hh, hd); glVertex3f(hw, hh, hd); glVertex3f(hw, hh, -hd); glVertex3f(-hw, hh, -hd);
    // Bawah
    glNormal3f(0, -1, 0); glVertex3f(-hw, -hh, -hd); glVertex3f(hw, -hh, -hd); glVertex3f(hw, -hh, hd); glVertex3f(-hw, -hh, hd);
    glEnd();
}

void drawCylinder(float radius, float height, int slices) {
    float halfH = height * 0.5f;
    // Sisi tabung
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; ++i) {
        float t = (float)i / slices * 2.0f * 3.14159265f;
        float x = cosf(t), z = sinf(t);
        glNormal3f(x, 0, z);
        glVertex3f(radius * x, -halfH, radius * z);
        glVertex3f(radius * x, halfH, radius * z);
    }
    glEnd();
    // Tutup Atas
    glBegin(GL_TRIANGLE_FAN); glNormal3f(0, 1, 0); glVertex3f(0, halfH, 0);
    for (int i = 0; i <= slices; ++i) {
        float t = (float)i / slices * 2.0f * 3.14159265f;
        glVertex3f(radius * cosf(t), halfH, radius * sinf(t));
    }
    glEnd();
    // Tutup Bawah
    glBegin(GL_TRIANGLE_FAN); glNormal3f(0, -1, 0); glVertex3f(0, -halfH, 0);
    for (int i = 0; i <= slices; ++i) {
        float t = (float)i / slices * 2.0f * 3.14159265f;
        glVertex3f(radius * cosf(t), -halfH, radius * sinf(t));
    }
    glEnd();
}

// ============= OBJEK LINGKUNGAN ============
void drawGround() {
    // Rumput
    glColor3f(0.30f, 0.50f, 0.30f);
    glBegin(GL_QUADS); glNormal3f(0, 1, 0);
    glVertex3f(-400, 0, -400); glVertex3f(400, 0, -400);
    glVertex3f(400, 0, 400); glVertex3f(-400, 0, 400);
    glEnd();

    // Paving Block U1 & U2
    glColor3f(COLOR_PAVING);
    glPushMatrix(); glTranslatef(0, 0.1f, 0);
    // Area U2
    glPushMatrix(); glTranslatef(-80, 0, -20); glScalef(60, 1, 50); drawBox(1, 0.1f, 1); glPopMatrix();
    // Area U1
    glPushMatrix(); glTranslatef(0, 0, 40); glScalef(70, 1, 30); drawBox(1, 0.1f, 1); glPopMatrix();
    glPopMatrix();
}

void drawTree(float x, float z) {
    glPushMatrix(); glTranslatef(x, 0, z);
    // Batang
    glColor3f(COLOR_TREE_TRUNK);
    glPushMatrix(); glTranslatef(0, 3.0f, 0); drawCylinder(1.0f, 6.0f, 8); glPopMatrix();
    // Daun
    glColor3f(COLOR_TREE_LEAVES);
    glPushMatrix(); glTranslatef(0, 7.0f, 0); glScalef(6, 6, 6); glutSolidSphere(1.0, 12, 12); glPopMatrix();
    glPopMatrix();
}

// ============= GEDUNG U1 (DEPAN) ============
void drawU1Front() {
    float baseY = 0.0f;
    // Podium Dasar (Krem)
    glColor3f(COLOR_FACADE_KREM);
    glPushMatrix(); glTranslatef(0, baseY + 1.5f, 0); drawCylinder(18.0f, 3.0f, 40); glPopMatrix();

    // Lantai Kaca + Sabuk Krem (Lantai 2 & 3)
    // Sabuk Bawah
    glColor3f(COLOR_FACADE_KREM);
    glPushMatrix(); glTranslatef(0, baseY + 3.2f, 0); drawCylinder(16.5f, 0.4f, 40); glPopMatrix();

    // Kaca Lantai 2
    glColor4f(COLOR_GLASS_WINDOW);
    glPushMatrix(); glTranslatef(0, baseY + 4.7f, 0); drawCylinder(16.0f, 2.6f, 40); glPopMatrix();

    // Sabuk Tengah
    glColor3f(COLOR_FACADE_KREM);
    glPushMatrix(); glTranslatef(0, baseY + 6.2f, 0); drawCylinder(16.5f, 0.4f, 40); glPopMatrix();

    // Kaca Lantai 3
    glColor4f(COLOR_GLASS_WINDOW);
    glPushMatrix(); glTranslatef(0, baseY + 7.7f, 0); drawCylinder(16.0f, 2.6f, 40); glPopMatrix();

    // Sabuk Atas
    glColor3f(COLOR_FACADE_KREM);
    glPushMatrix(); glTranslatef(0, baseY + 9.2f, 0); drawCylinder(16.5f, 0.4f, 40); glPopMatrix();

    // Kubah Kaca
    glColor4f(0.6f, 0.8f, 1.0f, 0.5f);
    glPushMatrix(); glTranslatef(0, baseY + 9.5f, 0); glScalef(6.0f, 2.0f, 6.0f); glutSolidSphere(1.0, 20, 20); glPopMatrix();
}

// ============= GEDUNG U1 (BELAKANG) ============
void drawU1Back() {
    glPushMatrix();
    glTranslatef(-20.0f, 0.0f, 5.0f);
    glRotatef(100.0f, 0, 1, 0);

    // Plinth Batu
    glColor3f(COLOR_PLINTH_STONE);
    glPushMatrix(); glTranslatef(0, 1.5f, 0); glScalef(12.0f, 3.0f, 24.0f); drawBox(1, 1, 1); glPopMatrix();

    // Loop Lantai (Krem - Kaca - Krem)
    for (int i = 0; i < 4; ++i) {
        float y = 3.0f + (i * 3.5f);

        // Lantai Krem (Beton)
        glColor3f(COLOR_FACADE_KREM);
        glPushMatrix(); glTranslatef(0, y, 0); glScalef(11.0f, 0.5f, 23.0f); drawBox(1, 1, 1); glPopMatrix();

        // Jendela Kaca (Di atas lantai krem)
        glColor4f(COLOR_GLASS_WINDOW);
        glPushMatrix(); glTranslatef(0, y + 1.75f, 0); glScalef(10.8f, 3.0f, 22.8f); drawBox(1, 1, 1); glPopMatrix();

        // Penutup Atas (Jika lantai terakhir)
        if (i == 3) {
            glColor3f(COLOR_FACADE_KREM);
            glPushMatrix(); glTranslatef(0, y + 3.5f, 0); glScalef(11.0f, 0.5f, 23.0f); drawBox(1, 1, 1); glPopMatrix();
        }
    }
    glPopMatrix();
}

// ============= GEDUNG U2 (PANJANG) ============
void drawU2() {
    glPushMatrix();
    glTranslatef(-80.0f, 0.0f, -40.0f);

    // Plinth Batu
    glColor3f(COLOR_PLINTH_STONE);
    glPushMatrix(); glTranslatef(-10.0f, 1.5f, 0); glScalef(15.0f, 3.0f, 18.0f); drawBox(1, 1, 1); glPopMatrix();

    // Loop Lantai (Struktur Lapis: Beton -> Kaca -> Beton)
    for (int i = 0; i < 4; ++i) {
        float y = 3.0f + (i * 3.5f);

        // Plat Lantai Krem (Lebih lebar dari kaca sedikit agar terlihat seperti 'sabuk')
        glColor3f(COLOR_FACADE_KREM);
        glPushMatrix(); glTranslatef(0, y, 0); glScalef(30.0f, 0.6f, 17.0f); drawBox(1, 1, 1); glPopMatrix();

        // Blok Kaca (Windows)
        glColor4f(COLOR_GLASS_WINDOW);
        glPushMatrix(); glTranslatef(0, y + 1.8f, 0); glScalef(29.5f, 3.0f, 16.5f); drawBox(1, 1, 1); glPopMatrix();

        // Atap Gedung
        if (i == 3) {
            glColor3f(COLOR_FACADE_KREM);
            glPushMatrix(); glTranslatef(0, y + 3.6f, 0); glScalef(30.0f, 0.6f, 17.0f); drawBox(1, 1, 1); glPopMatrix();
        }
    }
    glPopMatrix();
}

// ============= JEMBATAN ============
void drawU2Bridge() {
    glPushMatrix(); glTranslatef(-88.0f, 0.0f, -25.0f);
    // Kaca Vertikal
    glColor4f(COLOR_GLASS_WINDOW);
    glPushMatrix(); glTranslatef(15.0f, 10.0f, -5.0f); glScalef(1.0f, 14.0f, 10.0f); drawBox(1, 1, 1); glPopMatrix();
    // Sabuk Lantai
    glColor3f(COLOR_FACADE_KREM);
    for (int i = 1; i <= 4; i++) {
        glPushMatrix(); glTranslatef(15.0f, i * 3.5f + 1.0f, -5.0f); glScalef(1.2f, 0.4f, 10.2f); drawBox(1, 1, 1); glPopMatrix();
    }
    glPopMatrix();
}

// ============= KAMERA (YANG HILANG SEBELUMNYA) ============
void setCamera() {
    float yawRad = camYaw * DEG2RAD;
    float pitchRad = camPitch * DEG2RAD;

    float dirX = cosf(pitchRad) * sinf(yawRad);
    float dirY = sinf(pitchRad);
    float dirZ = -cosf(pitchRad) * cosf(yawRad);

    gluLookAt(camX, camY, camZ,
        camX + dirX, camY + dirY, camZ + dirZ,
        0, 1, 0);
}

// ============= DISPLAY ============
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set Kamera
    setCamera();

    // Cahaya
    GLfloat lightPos[] = { 50.0f, 150.0f, 100.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Gambar Objek
    drawGround();

    // Pohon (Posisi Aman)
    drawTree(-40, 20);
    drawTree(20, -20);
    drawTree(-130, -60);
    drawTree(-130, 0);

    drawU1Front();
    drawU1Back();
    drawU2();
    drawU2Bridge();

    // Koordinat Text
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glColor3f(1, 1, 0); glRasterPos2i(10, 580);
    char info[100]; sprintf_s(info, sizeof(info), "POS: %.1f, %.1f, %.1f", camX, camY, camZ);
    for (int i = 0; info[i]; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, info[i]);
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

// ============= INPUT & INIT ============
void keyboard(unsigned char key, int x, int y) {
    float speed = 2.0f;
    float yawRad = camYaw * DEG2RAD;
    float fx = sinf(yawRad), fz = -cosf(yawRad);
    float rx = cosf(yawRad), rz = sinf(yawRad);

    if (key == 27) exit(0);
    if (key == 'w') { camX += fx * speed; camZ += fz * speed; }
    if (key == 's') { camX -= fx * speed; camZ -= fz * speed; }
    if (key == 'a') { camX -= rx * speed; camZ -= rz * speed; }
    if (key == 'd') { camX += rx * speed; camZ += rz * speed; }
    if (key == 'q') camY += speed;
    if (key == 'e') camY -= speed;
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) camYaw -= 2.0f;
    if (key == GLUT_KEY_RIGHT) camYaw += 2.0f;
    if (key == GLUT_KEY_UP) camPitch += 2.0f;
    if (key == GLUT_KEY_DOWN) camPitch -= 2.0f;
    if (camPitch > 89) camPitch = 89;
    if (camPitch < -89) camPitch = -89;
    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

void initGL() {
    glClearColor(0.7f, 0.8f, 0.9f, 1.0f); // Langit Biru
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    // Transparansi Kaca
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Final Gedung Krem & Kaca");

    initGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}
