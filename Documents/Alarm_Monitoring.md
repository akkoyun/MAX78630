# Alarm İzleme

Her ölçüm aralığında işlemci tarafından otomatik olarak alarm koşulları belirlenir. Çip sıcaklığı, frekans, RMS voltaj değerleri kullanıcı tarafından yapılandırılabilen eşikleri aşar veta altına düşerse, STATUS registeri içerisinde ilgili bir alarm biti ayarlanır. RMS akım değerleri için ise aşırı akım koşullarını algılamak için kullanıcı tarafından belirlenen maksimum değer kullanılır. Güç katsayısı için ise kullanıcı tarafından belirlenen minimum değer kullanılır.

| Register | Açıklama                                                                   |
|----------|----------------------------------------------------------------------------|
| T_MAX    | Alarmı tetiklemek için Sıcaklığın aşması gereken eşik değeri.              |
| T_MIN    | Alarmı tetiklemek için Sıcaklığın altına düşmesi gereken eşik değeri.      |     
| F_MAX    | Alarmı tetiklemek için Frekansın aşması gereken eşik değeri.               |       
| F_MIN    | Alarmı tetiklemek için Frekansın altına düşmesi gereken eşik değeri.       |       
| VRMS_MAX | Alarmı tetiklemek için RMS Voltajının aşması gereken eşik değeri.          |       
| VRMS_MIN | Alarmı tetiklemek için RMS Voltajının altına düşmesi gereken eşik değeri.  |       
| IRMS_MAX | Alarmı tetiklemek için RMS akımının aşması gereken eşik değeri.            |       
| PF_MIN   | Alarmı tetiklemek için güç faktörünün altına düşmesi gereken eşik değeri.  |       

Üç voltaj ve üç akım dengesizliği, ilgili maksimum eşik V_IMB_MAX ve I_IMB_MAX'i aşmaları durumunda özel alarm bitleri aracılığıyla izlenir ve raporlanır.

| Register  | Açıklama                                                                         |
|-----------|----------------------------------------------------------------------------------|
| V_IMB_MAX | Alarmı tetiklemek için Voltaj Dengesizliğinin aşması gereken Yüzde Eşiği değeri. |
| I_IMB_MAX | Alarmı tetiklemek için Akım Dengesizliğinin aşması gereken Yüzde Eşiği değeri.   |     

STATUS register ayrıca SAG voltajı alarmları sağlar. 

| Register          | Açıklama                                                                                                                                          |
|-------------------|---------------------------------------------------------------------------------------------------------------------------------------------------|
| VSAG_LIM          | Bir Sarkma alarmını tetiklemek için voltajın altına düşmesi gereken eşik değeri (RMS cinsinden).                                                  |
| I_IMBVSAG_INT_MAX | Gerilimin eşiğin altında olması gereken aralık (örneklerde). Yarım döngü artışlarıyla ayarlanmalıdır (yani 60 Hz'de yarım döngü başına 22 örnek). |     
