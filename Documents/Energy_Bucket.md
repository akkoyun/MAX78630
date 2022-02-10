# Enerji Sayaçları için Kova Boyutu

BUCKET register kaydı, kullanıcının enerji sayacı kayıtları için ölçü birimi tanımlamasını sağlar. Tamsayı kısmı 24 bit ve kesir kısmı için 24 bit olan işaretsiz 48 bitlik sabit noktalı bir sayıdır.

![Bucket Data Structure](/Documents/Bucket_Data_Structure.png)

ölçü birimi, sayaçların çok hızlı taşmasını önleyecek kadar büyük ayarlanmalıdır. Örneğin, enerji sayaçlarını watt-saat cinsinden artırmak için, BUCKET'taki değer, bir saatteki saniye sayısı (3600) ile Örnekleme Hızı (2.7kS/s) çarpımı ve Tam Ölçekli Watt'a (VSCALE) bölünmelidir. x ISCALE).

$$Watt-hours (Wh) Bucket = (3600s * 2.7kS/s) / (VSCALE * ISCALE)$$

Tam Ölçekli Watt birimi, kullanılan donanım tasarımı tarafından tanımlanır (register ölçekleme bölümüne bakın). Örnek olarak, voltaj kaynakları tam ölçekte 400Vpk (VSCALE) ve akımlar tam ölçekte 30Apk (ISCALE) ise, tam ölçekli watt 12000 (VSCALE x ISCALE) olacaktır. Kova değeri, kayıt varsayılanı olarak flash belleğe kaydedilebilir.

**Örnekler:**

Yukarıda verilen ISCALE ve VSCALE değerleri için..

1. Wh birimi $3600 * 2700 / (400 * 30) = 810$ 'a eşittir. BUCKET kaydına yazılması gereken onaltılık değer, tamsayı kısmı ile hizalamak için 24 bit (223 ile çarpılarak) sola kaydırıldıktan sonra: **BUCKET = 0x00032A.000000**

2. KWh birimi $3600 * 2700 / (400 * 30 / 1000) = 810000$ 'a eşittir. BUCKET kaydına yazılması gereken onaltılık değer, hizalamak için 24 bit (224 ile çarpılarak) sola kaydırıldıktan sonra: **BUCKET = 0x0C5C10.000000**
