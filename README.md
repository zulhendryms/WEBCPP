# WEBCPP — Simple C++ HTTP Server (SPA)

Repository ini berisi server HTTP sederhana ditulis dengan C++ yang menyajikan sebuah Single Page Application (SPA) berisi:
- Halaman utama
- Game Snake
- Kalkulator sederhana

Semua fitur di-render oleh HTML/CSS/JS yang dikirim oleh server C++.

## Build & Run (lokal)

Pastikan Anda memiliki compiler C++ (g++) di Linux.

```bash
g++ server.cpp -o server -pthread
./server
# buka http://localhost:8080 di browser
```

Tekan Ctrl+C untuk menghentikan server.

## Perbaikan yang dibuat
- Pengecekan error untuk `socket`, `setsockopt`, `bind`, `listen`, `accept`.
- Menambahkan `SO_REUSEADDR` untuk restart cepat.
- Menyertakan header `Content-Length` dan `Connection: close`.
- Menggunakan thread per koneksi (detached) untuk melayani banyak klien.
- Menangani SIGINT agar server dapat dihentikan dengan bersih.
# WEBCPP
web with cpp
