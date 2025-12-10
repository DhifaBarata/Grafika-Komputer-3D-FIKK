README – Proyek Grafika Komputer 3d FIKK (OpenGL / GLUT)
🏫 Mata Kuliah: Grafika Komputer
👨‍🏫 Dosen Pengampu: Muhammad Sonhaji Akbar, S.Pd., M.Kom.
📅 Tahun Akademik: 2024/2025 PTI C
👥 Anggota Kelompok
No	Nama Lengkap	NIM
1	Ericha Okti Virlya Meyjie	24050974072
2	Fandi Prasetya	24050974078
3	Dhifa Barata Putra	24050974085 


FITUR YANG DIIMPLEMENTASIKAN
glTranslate - Untuk positioning setiap gedung dan objek
glRotate - Untuk rotasi kamera dan orientasi objek
glScale - Untuk mengatur ukuran gedung dan komponen
Interaksi Keyboard - Kontrol kamera dan navigasi
Interaksi Mouse - Rotasi view dan zoom
Color - Pewarnaan setiap komponen gedung
Lighting - Pencahayaan GL_LIGHT0
Shading - GL_SMOOTH dengan material specular dan shininess

KONTROL INTERAKSI
Keyboard Controls
Translasi Kamera
W : Gerak ke Atas (translateY +)
S : Gerak ke Bawah (translateY -)
A : Gerak ke Kiri (translateX -)
D : Gerak ke Kanan (translateX +)
Q : Zoom In (translateZ +)
E : Zoom Out (translateZ -)
Rotasi Manual
X : Rotasi sumbu X +5° Shift+X : Rotasi sumbu X -5°
Y : Rotasi sumbu Y +5° Shift+Y : Rotasi sumbu Y -5°
Z : Rotasi sumbu Z +5° Shift+Z : Rotasi sumbu Z -5°
Scale
+ : Zoom In (scaleFactor +)
- : Zoom Out (scaleFactor -)
Utility
R : Reset kamera ke posisi default
ESC : Keluar dari program
Arrow Keys
↑ : Zoom In 
↓ : Zoom Out 
← : Rotasi Kiri (angleY -) 
→ : Rotasi Kanan (angleY +)
Mouse Controls
Klik Kiri + Drag : Rotasi View (angleX & angleY) 
Scroll Up : Zoom In (scaleFactor +) 
Scroll Down : Zoom Out (scaleFactor -)

🛠 Tools & Library
Proyek dikembangkan menggunakan:
C++
OpenGL (GL)
GLU
GLUT / freeGLUT
Compiler: MinGW / GCC / Visual Studio C++
