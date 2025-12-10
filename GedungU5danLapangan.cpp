#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

// Variable global untuk transformasi
float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;
float translateX = 0.0f, translateY = 0.0f, translateZ = -50.0f;
float scaleFactor = 1.0f;

// Variable untuk interaksi mouse
int lastMouseX, lastMouseY;
bool isRotating = false;

// Variable untuk lighting 
GLfloat lightPos[] = { -60.0f, 25.0f, 35.0f, 1.0f }; 
GLfloat lightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat lightDiffuse[] = { 0.9f, 0.85f, 0.7f, 1.0f };
GLfloat lightSpecular[] = { 0.4f, 0.4f, 0.4f, 1.0f };

// Fungsi untuk inisialisasi
void init() {
    glClearColor(0.7f, 0.85f, 1.0f, 1.0f); // Warna background langit
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    // === SETUP LIGHTING ===
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);       

    // Set posisi dan properti cahaya
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Material properties
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Material specular untuk highlight
    GLfloat matSpecular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat matShininess[] = { 20.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 4.0 / 3.0, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

// Fungsi untuk menggambar balok dengan parameter (pos, scale, warna)
void drawBalok(float px, float py, float pz, float sx, float sy, float sz,
    float r, float g, float b) {
    glPushMatrix();
    glTranslatef(px, py, pz);
    glScalef(sx, sy, sz);
    glColor3f(r, g, b);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Fungsi untuk menggambar text 3D
void drawText3D(const char* text, float x, float y, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(scale, scale, scale);

    glLineWidth(3.0f);
    for (const char* c = text; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();
}

// Fungsi untuk menggambar jendela dengan detail
void drawJendela(float x, float y, float z, bool isFront) {
    float offsetZ = isFront ? 0.01f : -0.01f;

    // Frame jendela (hitam tipis)
    drawBalok(x, y, z + offsetZ, 1.4f, 1.8f, 0.15f, 0.15f, 0.15f, 0.15f);

    // Kaca jendela (biru gelap reflektif)
    drawBalok(x, y, z + offsetZ * 1.5f, 1.2f, 1.6f, 0.05f, 0.25f, 0.35f, 0.5f);

    // Pembagi vertikal tengah
    drawBalok(x, y, z + offsetZ * 2.0f, 0.1f, 1.6f, 0.05f, 0.15f, 0.15f, 0.15f);
}

// Fungsi untuk menggambar pintu
void drawPintu(float x, float y, float z) {
    // Frame pintu (coklat gelap)
    drawBalok(x, y, z + 0.2f, 2.2f, 3.8f, 0.3f, 0.25f, 0.2f, 0.15f); 

    // Kaca pintu bagian atas
    drawBalok(x, y + 1.5f, z + 0.35f, 2.0f, 1.5f, 0.1f, 0.3f, 0.4f, 0.5f); 

    // Panel pintu bagian bawah (coklat)
    drawBalok(x, y - 1.0f, z + 0.35f, 2.0f, 1.5f, 0.1f, 0.4f, 0.3f, 0.2f); 

    // Handle pintu
    drawBalok(x + 0.8f, y - 0.3f, z + 0.45f, 0.15f, 0.3f, 0.1f, 0.8f, 0.8f, 0.8f); 
}

// Fungsi untuk menggambar lantai dengan grid
void drawLantai() {
    // Lantai utama sebagai quad besar 
    glBegin(GL_QUADS);
    glColor3f(0.55f, 0.5f, 0.45f); // Warna paving
    glVertex3f(-85.0f, -12.0f, 35.0f);
    glVertex3f(45.0f, -12.0f, 35.0f);
    glVertex3f(45.0f, -12.0f, -110.0f);
    glVertex3f(-85.0f, -12.0f, -110.0f);
    glEnd();

    // Grid paving sebagai garis
    glColor3f(0.45f, 0.4f, 0.35f);
    glLineWidth(1.0f);

    for (float i = -85.0f; i <= 45.0f; i += 1.5f) {
        glBegin(GL_LINES);
        glVertex3f(i, -11.95f, -110.0f);
        glVertex3f(i, -11.95f, 35.0f);
        glEnd();
    }

    for (float i = -110.0f; i <= 35.0f; i += 1.5f) {
        glBegin(GL_LINES);
        glVertex3f(-85.0f, -11.95f, i);
        glVertex3f(45.0f, -11.95f, i);
        glEnd();
    }
}

// Fungsi untuk menggambar tiang
void drawTiang(float x, float y, float z) {
    drawBalok(x, y, z, 1.0f, 7.5f, 1.0f, 0.88f, 0.88f, 0.85f);
}

// Fungsi untuk menggambar kanopi entrance
void drawKanopi() {
    drawBalok(0.0f, -3.5f, 13.5f, 22.0f, 0.4f, 14.0f, 0.92f, 0.92f, 0.90f);

    // === GARIS-GARIS BALOK PADA KANOPI ===
    for (float i = -9.0f; i <= 9.0f; i += 2.2f) {
        drawBalok(i, -3.5f, 13.5f, 0.25f, 0.5f, 14.0f, 0.75f, 0.75f, 0.73f);
    }

    // === BORDER KANOPI DEPAN ===
    drawBalok(0.0f, -3.5f, 20.5f, 22.5f, 0.3f, 0.3f, 0.8f, 0.8f, 0.78f);

    // === BORDER KANOPI BELAKANG ===
    drawBalok(0.0f, -3.5f, 6.5f, 22.5f, 0.3f, 0.3f, 0.8f, 0.8f, 0.78f);

    // === TIANG KANOPI ===
    // Tiang depan
    drawTiang(-9.5f, -7.5f, 18.0f);
    drawTiang(9.5f, -7.5f, 18.0f);

    // Tiang belakang 
    drawTiang(-9.5f, -7.5f, 9.0f);
    drawTiang(9.5f, -7.5f, 9.0f);
}

// Fungsi untuk menggambar tangga
void drawTangga() {
    // Tangga depan 
    for (int i = 0; i < 5; i++) {
        float tinggi = -12.0f + (i * 0.35f);
        float kedalaman = 22.0f - (i * 0.5f); 

        // Anak tangga
        drawBalok(0.0f, tinggi, kedalaman, 20.0f, 0.25f, 1.0f, 0.75f, 0.75f, 0.72f);

        // Sisi anak tangga (lebih gelap)
        drawBalok(0.0f, tinggi - 0.125f, kedalaman - 0.5f, 20.0f, 0.02f, 0.02f, 0.65f, 0.65f, 0.62f);
    }
}

// Fungsi untuk menggambar gedung sayap kiri 
void drawSayapKiri() {
    // === BANGUNAN LANTAI 1 SAYAP KIRI ===
    drawBalok(-19.5f, -5.5f, 0.0f, 24.0f, 6.5f, 18.0f, 0.93f, 0.93f, 0.91f);

    // Grid vertikal pada dinding depan 
    for (float x = -31.0f; x <= -8.0f; x += 2.5f) {
        drawBalok(x, -5.5f, 9.1f, 0.15f, 6.5f, 0.1f, 0.78f, 0.78f, 0.76f);
    }

    // Grid horizontal 
    for (float y = -8.5f; y <= -2.5f; y += 2.0f) {
        drawBalok(-19.5f, y, 9.1f, 24.0f, 0.15f, 0.1f, 0.78f, 0.78f, 0.76f);
    }

    // JENDELA LANTAI 1 SAYAP KIRI (4 kolom x 2 baris)
    for (int i = 0; i < 4; i++) {
        drawJendela(-28.0f + (i * 5.0f), -4.0f, 9.0f, true);
    }
    for (int i = 0; i < 4; i++) {
        drawJendela(-28.0f + (i * 5.0f), -7.0f, 9.0f, true);
    }

    // === ATAP/LANTAI 2 SAYAP KIRI ===
    drawBalok(-19.5f, -2.0f, 0.0f, 24.5f, 0.8f, 18.5f, 0.88f, 0.88f, 0.86f);

    // Border atap
    drawBalok(-19.5f, -1.6f, 9.3f, 25.0f, 0.3f, 0.3f, 0.80f, 0.80f, 0.78f);

    // === LANTAI 2 SAYAP KIRI (bagian belakang, lebih rendah) ===
    drawBalok(-19.5f, -0.5f, -5.0f, 24.0f, 8.0f, 8.0f, 0.93f, 0.93f, 0.91f);

    // === JENDELA LANTAI 2 SAYAP KIRI BAGIAN DEPAN ===
    drawJendela(-27.5f, 0.5f, -0.9f, true);  // Jendela kiri
    drawJendela(-22.5f, 0.5f, -0.9f, true);  // Jendela tengah
    drawJendela(-17.5f, 0.5f, -0.9f, true);  // Jendela kanan

    // ATAP LANTAI 2 SAYAP KIRI ===
    drawBalok(-19.5f, 3.5f, -5.0f, 24.5f, 0.8f, 8.5f, 0.88f, 0.88f, 0.86f);

    // Border atap lantai 2 sayap kiri belakang
    drawBalok(-19.5f, 3.9f, -9.3f, 25.0f, 0.3f, 0.3f, 0.80f, 0.80f, 0.78f);

}

// Fungsi untuk menggambar gedung sayap kanan 
void drawSayapKanan() {
    // === BANGUNAN LANTAI 1 SAYAP KANAN ===
    drawBalok(19.5f, -5.5f, 0.0f, 24.0f, 6.5f, 18.0f, 0.93f, 0.93f, 0.91f);

    // Grid vertikal pada dinding depan 
    for (float x = 8.0f; x <= 31.0f; x += 2.5f) {
        drawBalok(x, -5.5f, 9.1f, 0.15f, 6.5f, 0.1f, 0.78f, 0.78f, 0.76f);
    }

    // Grid horizontal 
    for (float y = -8.5f; y <= -2.5f; y += 2.0f) {
        drawBalok(19.5f, y, 9.1f, 24.0f, 0.15f, 0.1f, 0.78f, 0.78f, 0.76f);
    }

    // JENDELA LANTAI 1 SAYAP KANAN (4 kolom x 2 baris)
    for (int i = 0; i < 4; i++) {
        drawJendela(13.0f + (i * 5.0f), -4.0f, 9.0f, true);
    }
    for (int i = 0; i < 4; i++) {
        drawJendela(13.0f + (i * 5.0f), -7.0f, 9.0f, true);
    }

    // === ATAP/LANTAI 2 SAYAP KANAN ===
    drawBalok(19.5f, -2.0f, 0.0f, 24.5f, 0.8f, 18.5f, 0.88f, 0.88f, 0.86f);

    // Border atap
    drawBalok(19.5f, -1.6f, 9.3f, 25.0f, 0.3f, 0.3f, 0.80f, 0.80f, 0.78f);

    // === LANTAI 2 SAYAP KANAN (bagian belakang, lebih rendah) ===
    drawBalok(19.5f, -0.5f, -5.0f, 24.0f, 8.0f, 8.0f, 0.93f, 0.93f, 0.91f);

    // === JENDELA LANTAI 2 SAYAP KANAN BAGIAN DEPAN ===
    drawJendela(17.5f, 0.5f, -0.9f, true);   
    drawJendela(22.5f, 0.5f, -0.9f, true);   
    drawJendela(27.5f, 0.5f, -0.9f, true);   

    // ATAP LANTAI 2 SAYAP KANAN
    drawBalok(19.5f, 3.5f, -5.0f, 24.5f, 0.8f, 8.5f, 0.88f, 0.88f, 0.86f);

    // Border atap lantai 2 sayap kanan belakang
    drawBalok(19.5f, 3.9f, -9.3f, 25.0f, 0.3f, 0.3f, 0.80f, 0.80f, 0.78f);
}

// Fungsi untuk menggambar bangunan tengah (lantai 2)
void drawBangunanTengah() {
    // === LANTAI 2 - BANGUNAN TENGAH ===
    drawBalok(0.0f, 3.5f, 0.0f, 26.0f, 9.5f, 18.0f, 0.93f, 0.93f, 0.91f);

    // Grid vertikal pada lantai 2
    for (float x = -12.0f; x <= 12.0f; x += 3.0f) {
        drawBalok(x, 3.5f, 9.1f, 0.15f, 9.5f, 0.1f, 0.78f, 0.78f, 0.76f);
    }

    // Grid horizontal pada lantai 2
    for (float y = -1.0f; y <= 8.0f; y += 3.0f) {
        drawBalok(0.0f, y, 9.1f, 26.0f, 0.15f, 0.1f, 0.78f, 0.78f, 0.76f);
    }

    // === PANEL LOGO UNIVERSITAS ===
    drawBalok(0.0f, 3.95f, 9.2f, 14.0f, 11.1f, 0.3f, 0.95f, 0.95f, 0.93f);

    // Logo universitas (kuning emas) 
    drawBalok(0.0f, 6.5f, 9.4f, 2.5f, 2.5f, 0.2f, 0.90f, 0.75f, 0.15f);

    // Detail logo (simplified)
    drawBalok(0.0f, 6.5f, 9.5f, 2.0f, 2.0f, 0.1f, 0.95f, 0.85f, 0.25f);

    // === TULISAN "FAKULTAS ILMU KEOLAHRAGAAN" ===
    glColor3f(0.1f, 0.1f, 0.1f); 
    glLineWidth(3.5f); 

    // FAKULTAS - baris pertama
    glPushMatrix();
    glTranslatef(-4.0f, 3.0f, 9.45f); 
    glScalef(0.011f, 0.011f, 0.011f);
    for (const char* c = "FAKULTAS"; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        glTranslatef(10.0f, 0.0f, 0.0f); 
    }
    glPopMatrix();

    // ILMU - baris kedua
    glPushMatrix();
    glTranslatef(-1.8f, 1.4f, 9.45f); 
    glScalef(0.011f, 0.011f, 0.011f);
    for (const char* c = "ILMU"; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        glTranslatef(10.0f, 0.0f, 0.0f); 
    }
    glPopMatrix();

    // KEOLAHRAGAAN - baris ketiga
    glPushMatrix();
    glTranslatef(-6.0f, -0.2f, 9.45f); 
    glScalef(0.011f, 0.011f, 0.011f);
    for (const char* c = "KEOLAHRAGAAN"; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        glTranslatef(10.0f, 0.0f, 0.0f); 
    }
    glPopMatrix();
    glLineWidth(1.0f); 

    // === ATAP PANEL LOGO ===
    drawBalok(0.0f, 9.5f, 9.2f, 14.5f, 0.8f, 0.8f, 0.88f, 0.88f, 0.86f);

    // Border atap panel logo
    drawBalok(0.0f, 9.9f, 9.5f, 15.0f, 0.3f, 0.3f, 0.80f, 0.80f, 0.78f);

    // === JENDELA LANTAI 2 - BARIS ATAS (kiri-kanan logo) ===
    drawJendela(-9.0f, 6.5f, 9.0f, true);
    drawJendela(-6.0f, 6.5f, 9.0f, true);
    drawJendela(6.0f, 6.5f, 9.0f, true);
    drawJendela(9.0f, 6.5f, 9.0f, true);

    // === JENDELA LANTAI 2 - BARIS TENGAH ===
    for (int i = -3; i <= 3; i++) {
        drawJendela(i * 3.5f, 3.5f, 9.0f, true);
    }

    // === JENDELA LANTAI 2 - BARIS BAWAH ===
    for (int i = -3; i <= 3; i++) {
        drawJendela(i * 3.5f, 0.5f, 9.0f, true);
    }

    // === ATAP LANTAI 2 ===
    drawBalok(0.0f, 8.5f, 0.0f, 27.0f, 1.0f, 18.5f, 0.88f, 0.88f, 0.86f);

    // Border atap
    drawBalok(0.0f, 9.0f, 9.3f, 27.5f, 0.3f, 0.3f, 0.80f, 0.80f, 0.78f);
}

// Fungsi untuk menggambar bangunan lantai 1 tengah
void drawLantai1Tengah() {
    // === BANGUNAN LANTAI 1 BAGIAN TENGAH ===
    drawBalok(0.0f, -5.5f, 0.0f, 26.0f, 6.5f, 18.0f, 0.93f, 0.93f, 0.91f);

    // Grid horizontal lantai 1
    for (float y = -8.5f; y <= -2.5f; y += 2.0f) {
        drawBalok(0.0f, y, 9.1f, 26.0f, 0.15f, 0.1f, 0.78f, 0.78f, 0.76f);
    }

    // === AREA ENTRANCE ===
    drawBalok(0.0f, -6.0f, 11.0f, 26.0f, 6.5f, 4.0f, 0.90f, 0.90f, 0.88f);

    // === PINTU UTAMA (3 pintu) ===
    drawPintu(-5.0f, -7.2f, 13.0f);
    drawPintu(0.0f, -7.2f, 13.0f);
    drawPintu(5.0f, -7.2f, 13.0f);

    // === ATAP/PLAFON LANTAI 1 TENGAH ===
    drawBalok(0.0f, -2.0f, 0.0f, 26.5f, 0.8f, 18.5f, 0.88f, 0.88f, 0.86f);

    // Border atap lantai 1 tengah
    drawBalok(0.0f, -1.6f, 9.3f, 27.0f, 0.3f, 0.3f, 0.80f, 0.80f, 0.78f);
}

// Fungsi untuk menggambar selasar/teras depan 
void drawSelasar() {
    drawBalok(0.0f, -10.25f, 14.0f, 26.0f, 0.5f, 12.0f, 0.82f, 0.82f, 0.80f);

    // === BORDER/LIS SELASAR (bagian depan) ===
    drawBalok(0.0f, -10.5f, 20.2f, 26.5f, 0.2f, 0.4f, 0.72f, 0.72f, 0.70f);

    // === BORDER SELASAR (bagian kiri) ===
    drawBalok(-13.0f, -10.5f, 14.0f, 0.4f, 0.2f, 12.0f, 0.72f, 0.72f, 0.70f);

    // === BORDER SELASAR (bagian kanan) ===
    drawBalok(13.0f, -10.5f, 14.0f, 0.4f, 0.2f, 12.0f, 0.72f, 0.72f, 0.70f);

    // === GARIS-GARIS POLA LANTAI SELASAR ===
    for (float x = -12.0f; x <= 12.0f; x += 4.0f) {
        drawBalok(x, -10.0f, 14.0f, 0.1f, 0.5f, 12.0f, 0.75f, 0.75f, 0.73f);
    }
}

// Fungsi untuk menggambar tiang bendera
void drawTiangBendera() {
    // Tiang bendera
    drawBalok(-17.0f, -3.5f, 23.0f, 0.25f, 16.0f, 0.25f, 0.75f, 0.75f, 0.75f);

    // Bendera Indonesia - Merah
    drawBalok(-15.0f, 3.5f, 23.0f, 3.5f, 1.2f, 0.05f, 1.0f, 0.0f, 0.0f);

    // Bendera Indonesia - Putih
    drawBalok(-15.0f, 2.5f, 23.0f, 3.5f, 1.2f, 0.05f, 1.0f, 1.0f, 1.0f);
}

// Fungsi untuk menggambar vegetasi
void drawVegetasi() {
    // POHON KIRI DEPAN
    drawBalok(-20.0f, -9.5f, 22.0f, 0.6f, 3.0f, 0.6f, 0.35f, 0.25f, 0.15f);
    drawBalok(-20.0f, -7.0f, 22.0f, 3.0f, 3.0f, 3.0f, 0.2f, 0.55f, 0.2f);

    // POHON KANAN DEPAN
    drawBalok(20.0f, -9.5f, 22.0f, 0.6f, 3.0f, 0.6f, 0.35f, 0.25f, 0.15f);
    drawBalok(20.0f, -7.0f, 22.0f, 3.0f, 3.0f, 3.0f, 0.2f, 0.55f, 0.2f);

    // SEMAK KECIL DI DEPAN ENTRANCE
    drawBalok(-12.0f, -11.2f, 22.0f, 2.5f, 1.2f, 1.5f, 0.25f, 0.6f, 0.25f);
    drawBalok(-8.5f, -11.2f, 25.0f, 2.0f, 1.0f, 1.2f, 0.25f, 0.6f, 0.25f);
    drawBalok(12.0f, -11.2f, 22.0f, 2.5f, 1.2f, 1.5f, 0.25f, 0.6f, 0.25f);
    drawBalok(8.5f, -11.2f, 25.0f, 2.0f, 1.0f, 1.2f, 0.25f, 0.6f, 0.25f);

    // TANAMAN KECIL DI SAMPING
    drawBalok(-35.0f, -11.0f, 5.0f, 1.5f, 1.5f, 1.5f, 0.3f, 0.65f, 0.3f);
    drawBalok(35.0f, -11.0f, 5.0f, 1.5f, 1.5f, 1.5f, 0.3f, 0.65f, 0.3f);
}

// Fungsi untuk menggambar mobil sederhana
void drawMobil(float x, float z, float r, float g, float b) {
    // Body mobil
    drawBalok(x, -10.5f, z, 3.0f, 1.2f, 5.0f, r, g, b);

    // Atap mobil
    drawBalok(x, -9.0f, z - 0.5f, 2.5f, 1.0f, 3.0f, r * 0.8f, g * 0.8f, b * 0.8f);

    // Kaca depan
    drawBalok(x, -9.0f, z - 2.0f, 2.3f, 0.8f, 0.2f, 0.3f, 0.4f, 0.5f);

    // Roda (sederhana sebagai balok)
    drawBalok(x - 1.2f, -11.2f, z - 1.5f, 0.6f, 0.6f, 0.6f, 0.1f, 0.1f, 0.1f);
    drawBalok(x + 1.2f, -11.2f, z + 1.5f, 0.6f, 0.6f, 0.6f, 0.1f, 0.1f, 0.1f);
}

// Fungsi untuk menggambar ring basket
void drawRingBasket(float x, float y, float z) {
    // Tiang ring
    drawBalok(x, y - 2.0f, z, 0.3f, 6.0f, 0.3f, 0.6f, 0.6f, 0.6f);

    // Papan pantul (backboard)
    drawBalok(x, y + 1.5f, z - 0.5f, 2.5f, 2.0f, 0.1f, 1.0f, 1.0f, 1.0f);

    // Ring (simplified sebagai balok tipis)
    drawBalok(x, y + 1.2f, z - 1.2f, 0.8f, 0.1f, 0.8f, 1.0f, 0.4f, 0.0f);
}

// Fungsi untuk menggambar tiang lampu
void drawTiangLampu(float x, float z) {
    // Tiang
    drawBalok(x, -2.0f, z, 0.4f, 18.0f, 0.4f, 0.3f, 0.3f, 0.3f);

    // Lampu di atas
    drawBalok(x, 7.5f, z, 1.2f, 0.8f, 1.2f, 0.2f, 0.2f, 0.2f);
}

// Fungsi untuk menggambar 1 lapangan basket
void drawLapanganBasket(float offsetX, float offsetZ) {
    // === LANTAI LAPANGAN (hijau/biru) ===
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.6f, 0.5f); // Warna lapangan outdoor
    glVertex3f(offsetX - 14.0f, -11.9f, offsetZ - 13.0f);
    glVertex3f(offsetX + 14.0f, -11.9f, offsetZ - 13.0f);
    glVertex3f(offsetX + 14.0f, -11.9f, offsetZ + 13.0f);
    glVertex3f(offsetX - 14.0f, -11.9f, offsetZ + 13.0f);
    glEnd();

    // === GARIS LAPANGAN (putih) ===
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);

    // Garis pinggir
    glBegin(GL_LINE_LOOP);
    glVertex3f(offsetX - 14.0f, -11.85f, offsetZ - 13.0f);
    glVertex3f(offsetX + 14.0f, -11.85f, offsetZ - 13.0f);
    glVertex3f(offsetX + 14.0f, -11.85f, offsetZ + 13.0f);
    glVertex3f(offsetX - 14.0f, -11.85f, offsetZ + 13.0f);
    glEnd();

    // Garis tengah
    glBegin(GL_LINES);
    glVertex3f(offsetX, -11.85f, offsetZ - 13.0f);
    glVertex3f(offsetX, -11.85f, offsetZ + 13.0f);
    glEnd();

    // Lingkaran tengah (simplified)
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 20; i++) {
        float angle = 2.0f * 3.14159f * i / 20;
        glVertex3f(offsetX + 3.0f * cos(angle), -11.85f, offsetZ + 3.0f * sin(angle));
    }
    glEnd();

    // === RING BASKET (2 di ujung lapangan) ===
    drawRingBasket(offsetX, -8.0f, offsetZ - 12.0f); // Ring sisi 1
    drawRingBasket(offsetX, -8.0f, offsetZ + 12.0f); // Ring sisi 2

    // === PAGAR DAN JARING DI SEKELILING ===
    float pagerHeight = 4.0f; // Setengah tinggi tiang lampu (8m)

    // TIANG PAGAR VERTIKAL - Sisi Kiri
    glColor3f(0.3f, 0.5f, 0.8f); // Biru
    for (float z = offsetZ - 14.0f; z <= offsetZ + 14.0f; z += 2.0f) {
        drawBalok(offsetX - 15.0f, -12.0f + pagerHeight / 2, z, 0.15f, pagerHeight, 0.15f, 0.3f, 0.5f, 0.8f);
    }

    // TIANG PAGAR VERTIKAL - Sisi Kanan
    for (float z = offsetZ - 14.0f; z <= offsetZ + 14.0f; z += 2.0f) {
        drawBalok(offsetX + 15.0f, -12.0f + pagerHeight / 2, z, 0.15f, pagerHeight, 0.15f, 0.3f, 0.5f, 0.8f);
    }

    // TIANG PAGAR VERTIKAL - Sisi Depan
    for (float x = offsetX - 15.0f; x <= offsetX + 15.0f; x += 2.0f) {
        drawBalok(x, -12.0f + pagerHeight / 2, offsetZ - 14.0f, 0.15f, pagerHeight, 0.15f, 0.3f, 0.5f, 0.8f);
    }

    // TIANG PAGAR VERTIKAL - Sisi Belakang
    for (float x = offsetX - 15.0f; x <= offsetX + 15.0f; x += 2.0f) {
        drawBalok(x, -12.0f + pagerHeight / 2, offsetZ + 14.0f, 0.15f, pagerHeight, 0.15f, 0.3f, 0.5f, 0.8f);
    }

    // === JARING/KAWAT (garis-garis horizontal dan vertikal) ===
    glColor3f(0.5f, 0.7f, 0.9f); // Biru muda untuk jaring
    glLineWidth(1.0f);

    // JARING HORIZONTAL - Sisi Kiri
    for (float y = -12.0f; y <= -12.0f + pagerHeight; y += 0.5f) {
        glBegin(GL_LINE_STRIP);
        for (float z = offsetZ - 14.0f; z <= offsetZ + 14.0f; z += 0.5f) {
            glVertex3f(offsetX - 15.0f, y, z);
        }
        glEnd();
    }

    // JARING VERTIKAL - Sisi Kiri
    for (float z = offsetZ - 14.0f; z <= offsetZ + 14.0f; z += 0.5f) {
        glBegin(GL_LINE_STRIP);
        for (float y = -12.0f; y <= -12.0f + pagerHeight; y += 0.5f) {
            glVertex3f(offsetX - 15.0f, y, z);
        }
        glEnd();
    }

    // JARING HORIZONTAL - Sisi Kanan
    for (float y = -12.0f; y <= -12.0f + pagerHeight; y += 0.5f) {
        glBegin(GL_LINE_STRIP);
        for (float z = offsetZ - 14.0f; z <= offsetZ + 14.0f; z += 0.5f) {
            glVertex3f(offsetX + 15.0f, y, z);
        }
        glEnd();
    }

    // JARING VERTIKAL - Sisi Kanan
    for (float z = offsetZ - 14.0f; z <= offsetZ + 14.0f; z += 0.5f) {
        glBegin(GL_LINE_STRIP);
        for (float y = -12.0f; y <= -12.0f + pagerHeight; y += 0.5f) {
            glVertex3f(offsetX + 15.0f, y, z);
        }
        glEnd();
    }

    // JARING HORIZONTAL - Sisi Depan
    for (float y = -12.0f; y <= -12.0f + pagerHeight; y += 0.5f) {
        glBegin(GL_LINE_STRIP);
        for (float x = offsetX - 15.0f; x <= offsetX + 15.0f; x += 0.5f) {
            glVertex3f(x, y, offsetZ - 14.0f);
        }
        glEnd();
    }

    // JARING VERTIKAL - Sisi Depan
    for (float x = offsetX - 15.0f; x <= offsetX + 15.0f; x += 0.5f) {
        glBegin(GL_LINE_STRIP);
        for (float y = -12.0f; y <= -12.0f + pagerHeight; y += 0.5f) {
            glVertex3f(x, y, offsetZ - 14.0f);
        }
        glEnd();
    }

    // JARING HORIZONTAL - Sisi Belakang
    for (float y = -12.0f; y <= -12.0f + pagerHeight; y += 0.5f) {
        glBegin(GL_LINE_STRIP);
        for (float x = offsetX - 15.0f; x <= offsetX + 15.0f; x += 0.5f) {
            glVertex3f(x, y, offsetZ + 14.0f);
        }
        glEnd();
    }

    // JARING VERTIKAL - Sisi Belakang
    for (float x = offsetX - 15.0f; x <= offsetX + 15.0f; x += 0.5f) {
        glBegin(GL_LINE_STRIP);
        for (float y = -12.0f; y <= -12.0f + pagerHeight; y += 0.5f) {
            glVertex3f(x, y, offsetZ + 14.0f);
        }
        glEnd();
    }

    // === TIANG LAMPU (4 di pojok) ===
    drawTiangLampu(offsetX - 16.0f, offsetZ - 15.0f);
    drawTiangLampu(offsetX + 16.0f, offsetZ - 15.0f);
    drawTiangLampu(offsetX - 16.0f, offsetZ + 15.0f);
    drawTiangLampu(offsetX + 16.0f, offsetZ + 15.0f);
}

// Fungsi untuk menggambar 2 lapangan basket
void drawLapanganArea() {
    // Lapangan 1 (kiri)
    drawLapanganBasket(-50.0f, -50.0f);

    // Lapangan 2 (kanan, bersebelahan)
    drawLapanganBasket(-50.0f, -80.0f);
}

// Fungsi utama untuk menggambar seluruh gedung
void drawGedung() {
    // Lantai/ground
    drawLantai();

    // Vegetasi
    drawVegetasi();

    // Mobil parkir
    drawMobil(-35.0f, 20.0f, 0.9f, 0.9f, 0.9f); // Putih kiri
    drawMobil(35.0f, 20.0f, 0.2f, 0.2f, 0.2f);  // Hitam kanan

    // Tiang bendera
    drawTiangBendera();

    // Tangga
    drawTangga();

    drawSelasar();

    // Kanopi entrance
    drawKanopi();

    // Bangunan sayap kiri dan kanan (DETAIL LENGKAP)
    drawSayapKiri();
    drawSayapKanan();

    // Bangunan tengah lantai 1
    drawLantai1Tengah();

    // Bangunan tengah lantai 2
    drawBangunanTengah();

    // === LAPANGAN BASKET (di belakang gedung U4) ===
    drawLapanganArea();
}

// Fungsi display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Transformasi kamera (perspective)
    glTranslatef(translateX, translateY, translateZ);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
    glScalef(scaleFactor, scaleFactor, scaleFactor);

    // Update posisi cahaya setelah transformasi
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Gambar gedung
    drawGedung();

    glutSwapBuffers();
}

// Fungsi keyboard untuk kontrol
void myKeyboard(unsigned char key, int x, int y) {
    switch (key) {
        // Rotasi
    case 'x': angleX += 5.0f; break;
    case 'X': angleX -= 5.0f; break;
    case 'y': angleY += 5.0f; break;
    case 'Y': angleY -= 5.0f; break;
    case 'z': angleZ += 5.0f; break;
    case 'Z': angleZ -= 5.0f; break;

        // Translasi
    case 'w': translateY += 1.0f; break;
    case 's': translateY -= 1.0f; break;
    case 'a': translateX -= 1.0f; break;
    case 'd': translateX += 1.0f; break;
    case 'q': translateZ += 1.0f; break;
    case 'e': translateZ -= 1.0f; break;

        // Scale
    case '+': scaleFactor += 0.1f; break;
    case '-': scaleFactor -= 0.1f; if (scaleFactor < 0.1f) scaleFactor = 0.1f; break;

        // Reset
    case 'r':
    case 'R':
        angleX = angleY = angleZ = 0.0f;
        translateX = translateY = 0.0f;
        translateZ = -50.0f;
        scaleFactor = 1.0f;
        break;

        // Exit (Esc)
    case 27: exit(0); break;
    }
    glutPostRedisplay();
}

// Fungsi special keyboard (arrow keys)
void mySpecialKeyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP: translateZ += 2.0f; break;
    case GLUT_KEY_DOWN: translateZ -= 2.0f; break;
    case GLUT_KEY_LEFT: angleY -= 5.0f; break;
    case GLUT_KEY_RIGHT: angleY += 5.0f; break;
    }
    glutPostRedisplay();
}

// Fungsi mouse untuk interaksi
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isRotating = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else {
            isRotating = false;
        }
    }

    // Scroll untuk zoom (freeglut: button 3/4)
    if (button == 3) { // Scroll up
        scaleFactor += 0.1f;
        glutPostRedisplay();
    }
    else if (button == 4) { // Scroll down
        scaleFactor -= 0.1f;
        if (scaleFactor < 0.1f) scaleFactor = 0.1f;
        glutPostRedisplay();
    }
}

// Fungsi motion untuk drag rotation
void motion(int x, int y) {
    if (isRotating) {
        angleY += (x - lastMouseX) * 0.5f;
        angleX += (y - lastMouseY) * 0.5f;
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

// Fungsi reshape
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Gedung FIKK UNESA");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecialKeyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}
