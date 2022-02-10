# Register Ölçekleme ve Sonuç Formatları

Tüm voltaj, akım ve güç verileri, çip içerisinde bir LSB'den (S.23 formatı) -1.0 ile 1.0 arasında bir değer aralığı ile binary-full-scale birimlerde depolanmaktadır. Voltaj ve akım değerleri için, tüm tam ölçekli kayıt okumaları, 250mVpk'lik (veya ön amplifikatörden 8x kazanç ile 31.25mVpk) maksimum analog girişe karşılık gelir. Örnek olarak, voltaj bölücünün girişindeki 230V-tepe değeri, çip girişinde 250mV-tepe değerini verirse, anlık voltaj için 1.0-LSB'lik bir tam ölçekli kayıt okuması elde edilir. Benzer şekilde, sensör girişindeki 30A, çip girişine 250mV sağlarsa, anlık akım için 1.0-1LSB'lik tam ölçekli kayıt değeri 30A'ya karşılık gelir. Tam ölçekli güç, tam ölçekli akım ve voltajın çarpım sonucuna karşılık gelir, bu nedenle bu örnekte tam ölçekli watt, 230V x 30A veya 6900 watt'tır.

Güç Faktörleri, bir LSB'den daha az -2 ila +2 aralığında ikili sabit noktalı sayı olarak rapor edilir (format S.22). 

Frekans verileri, bir LSB'den daha az 0 ila +256Hz aralığında ikili sabit noktalı sayı olarak rapor edilir (format S.16). 

Sıcaklık verileri, bir LSB'den daha az -16384°C ila +16384°C aralığında sabit bir ölçeklendirmeye sahiptir (format S.10). 

Enerji verilerinin ölçeklendirilmesi bu belgenin farklı bir bölümünde açıklanmıştır.

Kalıcı kayıtlar (IFSCALE ve VFSCALE), herhangi bir modül tasarımı için tam ölçekli kayıt okumalarına uygulanan gerçek dünya akım ve voltaj seviyelerini depolamak için sağlanmıştır. 

Herhangi bir ölçümleme uygulaması daha sonra ölçüm sonuçlarını gerektiği gibi herhangi bir veri formatına formatlayabilir. 

Bu kalıcı kayıtların kullanımı kullanıcı tanımlıdır ve içeriğinin cihazın dahili işlemleri üzerinde hiçbir etkisi yoktur.