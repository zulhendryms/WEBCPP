# Tutorial Deploy WEBCPP — GitHub → Docker → Render

Panduan lengkap deploy aplikasi C++ ke dunia tanpa VPS, hanya dengan GitHub dan Render.

## Alur Deployment

```
Local (git commit + push)
    ↓
GitHub (repo menerima push)
    ↓
GitHub Actions (build Docker image, push ke GHCR)
    ↓
Render (pull image dari GHCR, run container)
    ↓
Live di https://your-app.onrender.com
```

---

## Langkah 1: Siapkan GitHub Repository

### 1.1 Buat repo di GitHub (jika belum ada)

1. Buka https://github.com/new
2. Isi nama repo: **WEBCPP** (atau nama lain)
3. Pilih **Public** (agar Render bisa akses image di GHCR)
4. Buat repo
5. Copy URL repo (contoh: `https://github.com/USERNAME/WEBCPP.git`)

### 1.2 Push kode lokal ke GitHub

Di terminal, di folder `/workspaces/WEBCPP`:

```bash
git init
git add .
git commit -m "Initial commit: C++ web server with SPA"
git branch -M main
git remote add origin https://github.com/USERNAME/WEBCPP.git
git push -u origin main
```

Ganti `USERNAME` dengan username GitHub Anda.

**Verifikasi**: Buka https://github.com/USERNAME/WEBCPP — Anda akan melihat file `server.cpp`, `README.md`, `Dockerfile`, dll.

---

## Langkah 2: Aktifkan GitHub Actions dan GHCR

GitHub Actions workflow sudah ada di `.github/workflows/ci.yml`. Workflow ini otomatis build Docker image dan push ke GitHub Container Registry (GHCR) setiap kali ada push ke `main`.

### 2.1 Izin Permissions untuk Packages

1. Buka repo settings: **Settings** → **Actions** → **General**
2. Di bagian "Workflow permissions", pilih:
   - ☑ **Read and write permissions**
   - ☑ **Allow GitHub Actions to create and approve pull requests**
3. Klik **Save**

Ini memungkinkan GitHub Actions menulis image ke GHCR.

### 2.2 Trigger Workflow

Saat Anda melakukan `git push` ke `main`, GitHub Actions otomatis:
1. Build image Docker dari `Dockerfile`
2. Login ke GHCR
3. Push image ke `ghcr.io/USERNAME/WEBCPP:latest`

**Verifikasi workflow jalan**:
1. Buka repo: **Actions** tab
2. Lihat workflow "Build and publish Docker image" sedang berjalan atau selesai
3. Jika hijau ✓, image berhasil dipush ke GHCR

**Akses image di GHCR**:
- Buka https://github.com/USERNAME?tab=packages
- Anda akan melihat package `ghcr.io/USERNAME/WEBCPP`

---

## Langkah 3: Deploy ke Render

### 3.1 Buat akun Render

1. Buka https://render.com
2. Klik **Sign Up**
3. Pilih metode login (GitHub recommended untuk kemudahan)
4. Selesaikan setup

### 3.2 Deploy Web Service dari GHCR Image

1. Di dashboard Render, klik **Create** → **Web Service**
2. Pada opsi "Existing image?", pilih **Yes**
3. Isi Docker image URL:
   ```
   ghcr.io/USERNAME/WEBCPP:latest
   ```
4. Isi nama service: **webcpp** (atau nama lain)
5. Pilih region (contoh: Singapore, US, dsb)
6. Environment (opsional, biarkan kosong)
7. Klik **Create Web Service**

Render akan:
- Pull image dari GHCR
- Deploy container dengan expose port 8080
- Assign URL publik (contoh: `https://webcpp-xyz.onrender.com`)

**Tunggu hingga status "Live"** (biasanya 1-2 menit). Buka URL → Anda melihat aplikasi WEBCPP hidup di web!

### 3.3 Auto-redeploy saat push baru (Opsional)

Jika ingin Render otomatis deploy ulang setiap kali image baru di-push ke GHCR:

1. Di Render service settings, klik **Settings**
2. Cari opsi "Webhook" atau "Auto-deploy"
3. Copy Render Webhook URL
4. Di GitHub repo:
   - **Settings** → **Webhooks** → **Add webhook**
   - Paste URL Render
   - Event: **Push events**
   - Klik **Add webhook**

Sekarang setiap `git push` akan auto-deploy.

---

## Langkah 4: Verifikasi Aplikasi Hidup

1. Buka URL Render (contoh: `https://webcpp-xyz.onrender.com`)
2. Anda akan melihat halaman WEBCPP dengan:
   - Button navigasi (Home, Mini Game, Kalkulator)
   - Mini game (klik target)
   - Kalkulator sederhana
3. Test setiap fitur untuk memastikan semuanya berfungsi

---

## Troubleshooting

### Image gagal push ke GHCR
- Pastikan Permissions diaktifkan (Langkah 2.1)
- Lihat **Actions** tab untuk error logs
- Pastikan `Dockerfile` ada di root repo

### Render gagal pull image
- Verifikasi image URL benar: `ghcr.io/USERNAME/WEBCPP:latest`
- Pastikan repo public di GitHub
- Jika image tidak ada, trigger push baru ke GitHub (contoh: `git commit --allow-empty -m "Redeploy" && git push`)

### Port error atau app tidak berjalan
- Pastikan `server.cpp` listen di port 8080 (sudah ada di kode)
- Pastikan `Dockerfile` EXPOSE 8080
- Cek logs di Render: **Logs** tab

### Aplikasi lambat atau timeout
- Render free tier ada resource limits
- Upgrade ke paid plan jika butuh performa lebih baik

---

## File Workflow (`.github/workflows/ci.yml`)

Workflow ini sudah otomatis. Isinya:
- Trigger: push ke `main`
- Steps:
  1. Checkout kode
  2. Setup Docker Buildx
  3. Login ke GHCR dengan `GITHUB_TOKEN`
  4. Build dan push image ke `ghcr.io/USERNAME/WEBCPP:latest`

Tidak perlu edit file ini, tapi jika ingin custom (contoh: add deploy step), edit `.github/workflows/ci.yml`.

---

## Ringkasan Command (Copy-Paste)

### Setup lokal dan push ke GitHub
```bash
cd /workspaces/WEBCPP
git init
git add .
git commit -m "Initial commit"
git branch -M main
git remote add origin https://github.com/USERNAME/WEBCPP.git
git push -u origin main
```

### Update image (setelah ubah kode)
```bash
git add .
git commit -m "Update feature"
git push
# GitHub Actions otomatis build & push image
# Render otomatis redeploy (jika webhook aktif)
```

---

## Alternatif: Deploy ke Fly.io (tanpa mengubah langkah)

Jika Anda prefer Fly.io instead of Render:

### Instalasi & setup
```bash
curl -L https://fly.io/install.sh | sh
flyctl auth login
```

### Deploy image dari GHCR
```bash
flyctl launch --name webcpp --image ghcr.io/USERNAME/WEBCPP:latest
flyctl deploy
```

Fly.io akan assign URL (contoh: `https://webcpp.fly.dev`) dan run container otomatis.

---

## Support & Next Steps

- **Tambah domain custom**: Bisa atur di Render/Fly.io settings
- **Enable HTTPS**: Sudah default di Render & Fly.io (free)
- **Monitoring**: Lihat logs di dashboard Render/Fly.io
- **Update aplikasi**: Edit kode, commit, push → auto-deploy via GitHub Actions

Pertanyaan? Lihat logs di **GitHub Actions** atau **Render Logs** untuk debug.

Happy deploying! 🚀
