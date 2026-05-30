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

## GitHub Actions: build & push Docker image

Workshop ini menyertakan workflow GitHub Actions yang membangun image Docker dan mendorongnya ke GitHub Container Registry (GHCR).

1. Buat repository di GitHub dan push kode.
2. Aktifkan `Packages`/`GHCR` akses untuk token `GITHUB_TOKEN` (default memiliki izin push untuk paket di repo jika permissions diizinkan).
3. Workflow: `.github/workflows/ci.yml` akan membangun dan mendorong image `ghcr.io/<owner>/<repo>:latest` pada setiap push ke `main`.

Jika Anda ingin otomatis menjalankan container pada server Anda, Anda bisa menambahkan langkah deploy SSH di workflow dengan secrets `SSH_HOST`, `SSH_USER`, dan `SSH_KEY`.

## Manual deploy (contoh menggunakan Docker di server)

1. Taruh server (atau image Docker) di server remote.
2. Jika menggunakan Docker image yang dipush ke GHCR:

```bash
# di server remote
docker login ghcr.io -u USERNAME -p <PERSONAL_ACCESS_TOKEN>
docker pull ghcr.io/OWNER/REPO:latest
docker run -d --restart unless-stopped -p 8080:8080 ghcr.io/OWNER/REPO:latest
```

Ganti `OWNER/REPO` dengan nilai repo Anda.

## Pertanyaan atau pengembangan lanjut
- Ingin menambahkan routing file statis terpisah atau sertifikat TLS? Saya bisa bantu menambahkan `https` atau dukungan `Content-Type` dinamis.
# WEBCPP
web with cpp
