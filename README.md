# WEBCPP — Simple C++ HTTP Server (SPA)

Repository ini berisi server HTTP sederhana ditulis dengan C++ yang menyajikan sebuah Single Page Application (SPA) berisi:
- Halaman utama
- Mini game (klik target)
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

## Deployment

Aplikasi ini bisa di-deploy ke Render atau Fly.io tanpa VPS menggunakan GitHub Actions.

**[Lihat panduan lengkap di DEPLOY.md](DEPLOY.md)** untuk tutorial step-by-step:
1. Push repo ke GitHub
2. GitHub Actions otomatis build Docker image dan push ke GHCR
3. Deploy ke Render / Fly.io dengan sekali klik

## Quick Summary

- **Dockerfile**: Multi-stage build untuk image ringan (~80MB)
- **.github/workflows/ci.yml**: GitHub Actions otomatis build & push ke GHCR
- **DEPLOY.md**: Tutorial lengkap deploy ke Render atau Fly.io
# WEBCPP
web with cpp
