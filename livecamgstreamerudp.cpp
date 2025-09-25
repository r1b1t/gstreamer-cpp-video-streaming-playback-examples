#include <gst/gst.h>

int main(int argc, char *argv[])
{
    // Pipeline: tüm elementlerin zinciri (kamera -> sink)
    GstElement *pipeline;

    // Bus: pipeline içindeki elementlerin mesaj gönderdiği "haberleşme hattı"
    GstBus *bus;

    // Msg: bus üzerinden gelen tek bir mesajı temsil eder
    GstMessage *msg;

    // GStreamer başlat
    gst_init(&argc, &argv);

    // Pipeline oluştur (kamera kaynağı -> otomatik video sink)
    /*
    -ksvideosrc: windowsta kamerayı açan kod.

    -autovideosink:
    “Otomatik video sink” demektir.
    Senin sistemine uygun video sink’i seçer
    Windows’ta genelde Direct3D sink (d3d11videosink / directdrawsink) seçer.
    Linux’ta xvimagesink veya waylandsink seçer.

   */

    /*
    tee name=t : akışı ikiye böler, bu sayede hem yayınlayıp hemde ne yayınladığımızı görebiliriz
    udpsink host=239.255.42.42 port=5000 auto-multicast=true;
    host=239.255.42.42 → multicast adresi.
    port=5000 → multicast portu
    auto-multicast=true → GStreamer’ın socket’i multicast için ayarlamasını sağlar.
    */
    pipeline = gst_parse_launch(
        "ksvideosrc ! videoconvert ! tee name=t "
        "t. ! queue ! x264enc tune=zerolatency bitrate=800 speed-preset=ultrafast ! rtph264pay pt=96 ! udpsink host=239.255.42.42 port=5000 auto-multicast=true " // multicast yayını
        "t. ! queue ! x264enc tune=zerolatency bitrate=800 speed-preset=ultrafast ! rtph264pay pt=97 ! udpsink host=127.0.0.1 port=5001 "                         // unicast yayını
        "t. ! queue ! autovideosink",
        NULL);

    /*
    Sadece yayın yapar, yayın yapılan ekranı göstermez.
    pipeline = gst_parse_launch(
         "ksvideosrc ! videoconvert ! x264enc tune=zerolatency bitrate=500 speed-preset=ultrafast ! rtph264pay ! udpsink host=127.0.0.1 port=5000",
         NULL);

    */
    /*
    GStreamer’da x264enc → videoyu H.264 formatına sıkıştırır (codec).
    zerolatency → bu tamponlamayı minimuma indirir
    ultrafast: CPU çok az kullanır, hızlıdır ama sıkıştırma verimliliği düşük.

    UDP/TCP ile gönderebilmek için RTP (Real-time Transport Protocol) formatına çevirmemiz gerekiyor, bunu da rtph264pay yapıyor.
    */

    // Pipeline PLAYING duruma geçsin
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // Hata veya EOS (End of Stream) olana kadar bekle
    bus = gst_element_get_bus(pipeline); // Hata mesajları Bus denen bir haberleşme hattına gönderilir.
    msg = gst_bus_timed_pop_filtered(
        bus,                                                    // hangi hattan bekleyecek
        GST_CLOCK_TIME_NONE,                                    // sonsuza kadar bekle
        (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS)); // hangi mesaj gelince sonlanacak

    // Hata durumunda mesaj yazdır
    if (msg != NULL)
    {
        if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR)
        {
            GError *err;
            gchar *debug_info;
            gst_message_parse_error(msg, &err, &debug_info);
            g_printerr("Error: %s\n", err->message);
            g_error_free(err);
            g_free(debug_info);
        }
        gst_message_unref(msg);
    }

    // Kaynakları serbest bırak
    /*
    Kamera, dosya ya da ses cihazı açık kalabilir.
    Bellek sızıntısı (memory leak) olur.
    Program kapansa bile bazı kaynaklar kilitli kalabilir.
    */
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}
