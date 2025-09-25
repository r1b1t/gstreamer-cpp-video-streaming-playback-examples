🇹🇷 Türkçe Proje Açıklaması:

Bu proje, GStreamer kütüphanesi kullanılarak hazırlanmış C++ örnek uygulamalarını içermektedir.
Amaç; medya oynatma, kamera görüntüsü yakalama, gerçek zamanlı yayın yapma (multicast/unicast) gibi temel senaryoları göstermektir.

📂 İçerik
Video Oynatma (playbin)
Yerel dosyadan video oynatmayı gösterir.
sintel_trailer-480p.webm örnek dosyası üzerinden çalışır.

Kamera Görüntüsü Alma
Windows’ta ksvideosrc ile kamera kaynağını açar.
autovideosink kullanarak platforma uygun video çıkışını otomatik seçer.

Gerçek Zamanlı Yayın (Multicast & Unicast)
Kameradan alınan görüntüyü H.264 formatına sıkıştırır.
Hem multicast (239.255.42.42:5000) hem unicast (127.0.0.1:5001) olarak yayınlar.
Aynı anda ekranda da görüntüyü gösterir.

🚀 Kullanım
GStreamer kurulumunun yapılmış olması gerekir.

🇬🇧 English Project Description:

This project contains C++ example applications developed using the GStreamer library.
The goal is to demonstrate fundamental scenarios such as media playback, capturing camera input, and real-time streaming (multicast/unicast).

📂 Contents
Video Playback (playbin)
Demonstrates playing a local video file.
Uses the sintel_trailer-480p.webm sample file.

Camera Capture
Opens the camera source on Windows using ksvideosrc.
Uses autovideosink to automatically select the appropriate video sink for the platform.

Real-Time Streaming (Multicast & Unicast)
Captures video from the camera and encodes it into H.264 format.
Streams simultaneously via multicast (239.255.42.42:5000) and unicast (127.0.0.1:5001).
Also displays the video locally on the screen.

🚀 Usage
Requires a working installation of GStreame
