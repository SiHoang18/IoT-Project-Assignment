#include "tinyml.h"
#include "global.h"
#include "dht_anormaly_model.h"
#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/c/common.h"

const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
tflite::ErrorReporter* error_reporter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

constexpr int kTensorArenaSize = 8 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

void initTinyML() {
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    model = tflite::GetModel(dht_anomaly_model_tflite);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        return;
    }

    static tflite::AllOpsResolver resolver;
    static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        return;
    }

    input = interpreter->input(0);
    output = interpreter->output(0);

    xTaskCreate(inferenceTask, "TinyML_Task", 4096, NULL, 1, NULL);
}

void inferenceTask(void *pvParameters) {
    while (1) {
        float currentTemp = 0.0;
        float currentHum = 0.0;

        if (xSemaphoreTake(sensor_mutex, portMAX_DELAY) == pdTRUE) {
            currentTemp = glob_temperature;
            currentHum = glob_humidity;
            xSemaphoreGive(sensor_mutex);
        }

        input->data.f[0] = currentTemp;
        input->data.f[1] = currentHum;

        unsigned long startTime = micros();
        TfLiteStatus invoke_status = interpreter->Invoke();
        unsigned long endTime = micros();

        if (invoke_status == kTfLiteOk) {
            float prediction = output->data.f[0];
            Serial.print("Prediction: ");
            Serial.print(prediction);
            Serial.print(" | Time(us): ");
            Serial.println(endTime - startTime);
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}