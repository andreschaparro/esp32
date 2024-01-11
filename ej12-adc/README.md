# Uso de los ADC del ESP32 en ESP-IDF

## Introduccion

El ESP32 tiene dos modulos ADC llamados _ADC_UNIT_1_ y _ADC_UNIT_2_ de 12 bits.

NOTA: El _ADC_UNIT_2_ puede no funcinonar correctamente en ciertas condiciones, por lo que debe consultarse la documentacion al utilizarlo.

Ambos ADC del ESP32 puede leer tensiones de 0 a 3,3V. Las que convierte en valores de 0 a 4095.

Sin embargo, tienen un comportamiento **alineal**. Tal es asi, que 3,3V y 3,2V daran el mismo valor de 4095:

![Alinealidad](/curva.png "Alinealidad")

Los canales del _ADC_UNIT_1_ son:

- **ADC_CHANNEL_0** mapeado en **GPIO36**.
- **ADC_CHANNEL_3** mapeado en **GPIO39**.
- **ADC_CHANNEL_4** mapeado en **GPIO32**.
- **ADC_CHANNEL_5** mapeado en **GPIO33**.
- **ADC_CHANNEL_6** mapeado en **GPIO34**.
- **ADC_CHANNEL_7** mapeado en **GPIO35**.

Bibliotecas a incluir:

- **esp_adc/adc_oneshot.h**
- **esp_adc/adc_cali_scheme.h**

## Inicializacion del ADC_UNIT_1

1. Crear una variable llamada _adc1_handle_ del tipo _adc_oneshot_unit_handle_t_.
2. Crear una variable llamada _adc1_init_cfg_ del tipò _adc_oneshot_unit_init_cfg_t_:
   - El miembro _unit_id_ debe valer _ADC_UNIT_1_.
   - El miembro _ulp_mode_ debe valer _ADC_ULP_MODE_DISABLE_.
3. Llamar a la funcion _adc_oneshot_new_unit_.
4. Pasarle como primer parametro un puntero a _adc1_init_cfg_.
5. Pasarle como segundo parametro un puntero a _adc1_handle_.
6. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.
7. Crear una variable llamada _adc1_chan_cfg_ del tipo _adc_oneshot_chan_cfg_t_:
   - El miembro _bitwidth_ debe valer _ADC_BITWIDTH_12_.
   - El miembro _atten_ debe valer _ADC_ATTEN_DB_11_.

## Inicializacion de los canales

1. Llamar a la funcion _adc_oneshot_config_channel_.
2. Pasarle como primer parametro _adc1_handle_.
3. Pasarle como segundo parametro el numero de canal. En este caso, _ADC_CHANNEL_0_.
4. Pasarle como tercer parametro un puntero a _adc1_chan_cfg_.

NOTA: Repetir para cada canal lo anterior.

## Calibracion para poder leer el valor de conversion en mV

1. Crear una variable llamada _cali_handle_ del tipo _adc_cali_handle_t_ en _NULL_.
2. Crear una variable llamada _cali_config_ del tipo _adc_cali_line_fitting_config_t_:
   - El miembro _unit_id_ debe valer _ADC_BITWIDTH_12_.
   - El miembro _atten_ debe valer _ADC_ATTEN_DB_11_.
   - El miembro _bitwidth_ debe valer _ADC_BITWIDTH_12_.
3. Llamar a la funcion _adc_cali_create_scheme_line_fitting_.
4. Pasarle como primer parametro un puntero a _cali_config_.
5. Pasarle como segundo parametro un puntero a _cali_handle_.
6. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

## Leer el valor de 0 a 4095 de un canal

1. Crear una variable llamada _raw_result_ del tipo _int_.
2. Llamar a la funcion _adc_oneshot_read_.
3. Pasarle como primer parametro _adc1_handle_.
4. Pasarle como segundo parametro el numero de canal. En este caso, _ADC_CHANNEL_0_.
5. Pasarle como tercer parametro un puntero a _raw_result_.
6. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

El resultado quedara almacenado en la variable _raw_result_.

## Leer el valor en mV de un canal

1. Crear una variable llamada _mv_result_ del tipo _int_.
2. Llamar a la funcion _adc_cali_raw_to_voltage_.
3. Pasarle como primer parametro _cali_handle_.
4. Pasarle como segundo parametro _raw_result_.
5. Pasarle como tercer parametro un puntero a _mv_result_.
6. Verificar el valor retornado con la macro _ESP_ERROR_CHECK_.

El resultado quedara almacenado en la variable _mv_result_.

## Ruido

Los ADC del ESP32 son muy sensibles al ruido. Se recomienda conectar un capacitor de bypass de 100nF lo mas cerca posible del pad de entrada.

Otra opcion, es almacenar varias muestras en un buffer circular e ir promediando la sumatoria de las mismas (Filtro Digital FIR).

![Ruido](/ruido.png "Ruido")
