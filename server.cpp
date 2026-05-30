#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <csignal>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

static int server_fd = -1;
static std::atomic<bool> running{true};

void handle_sigint(int) {
        running = false;
        if (server_fd != -1) close(server_fd);
}

void send_all(int sock, const char *buf, size_t len) {
        size_t sent = 0;
        while (sent < len) {
                ssize_t n = send(sock, buf + sent, len - sent, 0);
                if (n <= 0) break;
                sent += n;
        }
}

void handle_client(int client_sock) {
        // baca request (tidak mem-parsing lengkap, cukup untuk mengosongkan socket)
        char buffer[4096];
        ssize_t r = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        (void)r; // kita tidak butuh isi request untuk SPA ini

        // Body HTML (Single Page App: home + mini-game + kalkulator)
        const std::string body = R"rawliteral(<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width,initial-scale=1" />
    <title>WEBCPP - SPA</title>
    <style>
        :root{--bg:#eef2f7;--card:#fff;--accent:#3498db;--accent-dark:#2980b9}
        body{font-family:Inter,Arial,sans-serif;background:var(--bg);margin:0;color:#222}
        header{background:#34495e;color:#fff;padding:18px;text-align:center}
        nav{display:flex;gap:8px;justify-content:center;padding:12px}
        nav button{background:var(--card);border:1px solid #ddd;padding:8px 14px;border-radius:8px;cursor:pointer}
        nav button.active{background:var(--accent);color:#fff;border-color:transparent}
        .container{max-width:900px;margin:28px auto;padding:20px}
        .card{background:var(--card);padding:20px;border-radius:12px;box-shadow:0 6px 18px rgba(0,0,0,0.06)}
        .hidden{display:none}
        /* Game styles */
        #gameCanvas{background:#0d47a1;border-radius:8px;display:block;margin:12px auto}
        #score{font-weight:700}
        /* Calculator */
        .calc{max-width:320px;margin:10px auto}
        .display{height:46px;background:#f3f3f3;padding:8px;border-radius:6px;text-align:right;font-size:20px}
        .keys{display:grid;grid-template-columns:repeat(4,1fr);gap:8px;margin-top:8px}
        .keys button{padding:14px;border-radius:6px;border:1px solid #ddd;background:#fff;cursor:pointer}
    </style>
</head>
<body>
    <header><h1>WEBCPP — Single Page App (C++ Server)</h1></header>
    <nav>
        <button data-view="home" class="active">Home</button>
        <button data-view="game">Mini Game</button>
        <button data-view="calc">Kalkulator</button>
    </nav>

    <main class="container">
        <section id="home" class="card">
            <h2>Selamat datang</h2>
            <p>Aplikasi ini disajikan oleh server C++ sederhana. Gunakan tab di atas untuk berpindah antar fitur tanpa reload.</p>
            <p>Fitur: <b>Mini Game</b> (klik target untuk skor) dan <b>Kalkulator</b>.</p>
        </section>

        <section id="game" class="card hidden">
            <h2>Mini Game — Klik Target</h2>
            <p>Skor: <span id="score">0</span></p>
            <canvas id="gameCanvas" width="600" height="300"></canvas>
            <div style="text-align:center;margin-top:12px">
                <button id="startBtn">Mulai</button>
                <button id="resetBtn">Reset</button>
            </div>
        </section>

        <section id="calc" class="card hidden">
            <h2>Kalkulator Sederhana</h2>
            <div class="calc">
                <div id="display" class="display">0</div>
                <div class="keys">
                    <button data-key="7">7</button><button data-key="8">8</button><button data-key="9">9</button><button data-op="/">÷</button>
                    <button data-key="4">4</button><button data-key="5">5</button><button data-key="6">6</button><button data-op="*">×</button>
                    <button data-key="1">1</button><button data-key="2">2</button><button data-key="3">3</button><button data-op="-">−</button>
                    <button data-key="0">0</button><button data-key=".">.</button><button id="eq">=</button><button data-op="+">+</button>
                </div>
            </div>
        </section>
    </main>

    <script>
        // Simple SPA navigation
        const buttons = document.querySelectorAll('nav button');
        buttons.forEach(b=>b.addEventListener('click',()=>{
            document.querySelectorAll('nav button').forEach(x=>x.classList.remove('active'));
            b.classList.add('active');
            document.querySelectorAll('main section').forEach(s=>s.classList.add('hidden'));
            document.getElementById(b.dataset.view).classList.remove('hidden');
        }));

        // --- Mini Game ---
        const canvas = document.getElementById('gameCanvas');
        const ctx = canvas.getContext('2d');
        let score = 0; let running = false; let target = {x:300,y:150,r:20}; let anim;
        const scoreEl = document.getElementById('score');

        function draw(){
            ctx.clearRect(0,0,canvas.width,canvas.height);
            // draw target
            ctx.beginPath(); ctx.fillStyle = '#ff5252'; ctx.arc(target.x,target.y,target.r,0,Math.PI*2); ctx.fill();
            ctx.closePath();
            if(running) anim = requestAnimationFrame(()=>{ // move slowly
                target.x += (Math.random()-0.5)*80;
                target.y += (Math.random()-0.5)*60;
                target.x = Math.max(20, Math.min(canvas.width-20, target.x));
                target.y = Math.max(20, Math.min(canvas.height-20, target.y));
                draw();
            });
        }

        canvas.addEventListener('click', (e)=>{
            const rect = canvas.getBoundingClientRect();
            const x = e.clientX - rect.left; const y = e.clientY - rect.top;
            const d = Math.hypot(x-target.x, y-target.y);
            if (d <= target.r) { score += 1; scoreEl.textContent = score; }
        });

        document.getElementById('startBtn').addEventListener('click',()=>{ if(!running){running=true; draw();}});
        document.getElementById('resetBtn').addEventListener('click',()=>{ score=0; scoreEl.textContent=score; running=false; cancelAnimationFrame(anim); ctx.clearRect(0,0,canvas.width,canvas.height);});

        // --- Kalkulator ---
        const display = document.getElementById('display');
        let current = '';
        document.querySelectorAll('.keys button').forEach(b=>{
            b.addEventListener('click', ()=>{
                if (b.dataset.key) {
                    if (current === '0' && b.dataset.key !== '.') current = b.dataset.key; else current += b.dataset.key;
                    display.textContent = current;
                } else if (b.dataset.op) {
                    if (current === '') return; current += b.dataset.op; display.textContent = current;
                }
            });
        });
        document.getElementById('eq').addEventListener('click', ()=>{
            try { const result = Function('return '+current)(); display.textContent = String(result); current = String(result); } catch(e){ display.textContent = 'Error'; current=''; }
        });

    </script>
</body>
</html>)rawliteral";

        // Build HTTP response with Content-Length and close connection
        std::string headers = "HTTP/1.1 200 OK\r\n";
        headers += "Content-Type: text/html; charset=utf-8\r\n";
        headers += "Content-Length: ";
        headers += std::to_string(body.size());
        headers += "\r\nConnection: close\r\n\r\n";

        std::string response = headers + body;
        send_all(client_sock, response.c_str(), response.size());
        close(client_sock);
}

int main() {
        signal(SIGINT, handle_sigint);
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
                std::perror("socket");
                return 1;
        }

        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
                std::perror("setsockopt");
                close(server_fd);
                return 1;
        }

        struct sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(8080);

        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
                std::perror("bind");
                close(server_fd);
                return 1;
        }

        if (listen(server_fd, 16) < 0) {
                std::perror("listen");
                close(server_fd);
                return 1;
        }

        std::cout << "Server jalan di http://localhost:8080\n";

        std::vector<std::thread> workers;
        while (running) {
                struct sockaddr_in client_addr{};
                socklen_t client_len = sizeof(client_addr);
                int client_sock = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                if (client_sock < 0) {
                        if (!running) break;
                        std::perror("accept");
                        continue;
                }

                // handle each client in a detached thread
                std::thread t(handle_client, client_sock);
                t.detach();
        }

        if (server_fd != -1) close(server_fd);
        std::cout << "Server dihentikan\n";
        return 0;
}
