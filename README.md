## Health Monitoring system 

Sensors are interfaced to the embedded world which constantly records data and the data is fetched through server and analyzed through graphical representation. The attached sensors on patient’s body form a wireless body sensor network 
the heart rate, blood pressure and so on. The project can detect the abnormal conditions, issue an alarm to the patient and present a graphical view to the physician

Sensors such as EG828 , LM35 and Digital Heartbeat sensor are interfaced and data is transfered to PC via USB to TTL converter. For further details , Kindly refer to the technical document.

Internally , 12 Bit ADC is used for conversion of values from analog to digital. Therefore , while calcuating temperature , the formula that is implemented is 
= 3.3 * 1000 /4095 (Where 3.3 V is the Internal Compartor voltage to ADC)



