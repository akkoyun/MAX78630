# Çip Sıcaklığı Kalibrasyonu

**MAX78630**, voltaj referans hatalarını (sıcaklık nedeni ile oluşabilecek) izlemek için ölçüm işlemcisi tarafından kullanılabilen ve **TEMPC** registeri ile kullanıcıya sunulan dahili bir çip sıcaklık sensörüne sahiptir.

**COMMAND** registeri içerisinde yer alan **TC** bitinin ayarlanması (True: etkin), üretici yazılımının ölçülen çip sıcaklığına dayalı olarak daha hassas ayar yapmasına izin verir. $T = (T_g * T_0) + T_o$. Sıcaklık ofseti kullanıcı tarafından **T_OFFS** registeri yardımı ile kalibre edilebilir. Sıcaklık kazancı ise **T_GAIN** registeri yardımı ile kullanıcı tarafından kalibre edilebilir olduğu gibi aynı zamanda fabrika çıkışı olarak ayarlanmıştır.

| Register | Açıklama                                 |       |
|----------|------------------------------------------|-------|
| T        | Çip sıcaklığı kalibre değeri.            |       |
| T_OFFS   | Çip sıcaklığı offset kalibrasyon değeri. | $T_o$ |
| T_GAIN   | Çip sıcaklığı kazanç kalibrasyon değeri. | $T_g$ |

