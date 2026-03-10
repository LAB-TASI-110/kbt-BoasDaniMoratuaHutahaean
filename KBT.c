#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef struct {
    char origin[50];
    char destination[50];
    float base_fare_per_kg;
    float base_fare_per_dimension;
} Route;

Route routes[] = {
    {"Medan", "Tarutung", 5000.0, 10.0},
    {"Medan", "Porsea", 4500.0, 9.0},
    {"Tarutung", "Medan", 5000.0, 10.0},
    {"Porsea", "Medan", 4500.0, 9.0},
    {"Medan", "Jakarta", 15000.0, 25.0},
    {"Jakarta", "Bandung", 8000.0, 15.0}
};
int num_routes = sizeof(routes) / sizeof(routes[0]);

Route* find_route(const char* origin, const char* destination) {
    for (int i = 0; i < num_routes; i++) {
        if (strcasecmp(routes[i].origin, origin) == 0 && strcasecmp(routes[i].destination, destination) == 0) {
            return &routes[i];
        }
    }
    return NULL;
}

void generate_tracking_number(char* buffer, int buffer_size) {
    time_t t;
    srand((unsigned int)time(&t));
    snprintf(buffer, buffer_size, "KBT-%04d%04d%04d", rand() % 10000, rand() % 10000, rand() % 10000);
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void display_available_routes() {
    printf("\n--- Rute Tersedia ---\n");
    for (int i = 0; i < num_routes; i++) {
        printf("%d. %s - %s (Tarif/kg: %.2f, Tarif/cm³: %.2f)\n",
               i + 1, routes[i].origin, routes[i].destination,
               routes[i].base_fare_per_kg, routes[i].base_fare_per_dimension);
    }
    printf("----------------------\n");
}

int main() {
    printf("===========================================\n");
    printf("  Sistem Simulasi Kalkulasi Kargo KBT      \n");
    printf("===========================================\n");

    display_available_routes();

    char origin_input[50];
    char dest_input[50];
    float weight_kg;
    float length_cm, width_cm, height_cm;
    float dimension_cm3 = 0;
    float calculated_fare;
    char tracking_num[50];
    Route* selected_route = NULL;

    while (selected_route == NULL) {
        printf("\nMasukkan Kota Asal Pengiriman (lihat daftar di atas): ");
        scanf("%49s", origin_input);
        clear_input_buffer();

        printf("Masukkan Kota Tujuan Pengiriman (lihat daftar di atas): ");
        scanf("%49s", dest_input);
        clear_input_buffer();

        selected_route = find_route(origin_input, dest_input);

        if (selected_route == NULL) {
            printf("[Peringatan]: Rute '%s - %s' tidak ditemukan. Mohon periksa kembali input Anda.\n", origin_input, dest_input);
            printf("Silakan masukkan rute yang sesuai dengan daftar di atas.\n");
        }
    }

    printf("\nMasukkan Berat Barang (kg): ");
    while (scanf("%f", &weight_kg) != 1 || weight_kg <= 0) {
        printf("[Peringatan]: Input berat tidak valid. Masukkan angka positif: ");
        clear_input_buffer();
    }
    clear_input_buffer();

    printf("\nApakah ada dimensi barang? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    clear_input_buffer();

    if (tolower(choice) == 'y') {
        printf("Masukkan Dimensi Barang (panjang x lebar x tinggi dalam cm):\n");
        printf("Panjang (cm): ");
        while (scanf("%f", &length_cm) != 1 || length_cm <= 0) {
            printf("[Peringatan]: Input panjang tidak valid. Masukkan angka positif: ");
            clear_input_buffer();
        }
        clear_input_buffer();

        printf("Lebar (cm): ");
        while (scanf("%f", &width_cm) != 1 || width_cm <= 0) {
            printf("[Peringatan]: Input lebar tidak valid. Masukkan angka positif: ");
            clear_input_buffer();
        }
        clear_input_buffer();

        printf("Tinggi (cm): ");
        while (scanf("%f", &height_cm) != 1 || height_cm <= 0) {
            printf("[Peringatan]: Input tinggi tidak valid. Masukkan angka positif: ");
            clear_input_buffer();
        }
        clear_input_buffer();
        dimension_cm3 = length_cm * width_cm * height_cm;
    }

    float fare_by_weight = weight_kg * selected_route->base_fare_per_kg;
    float fare_by_dimension = dimension_cm3 * selected_route->base_fare_per_dimension;

    if (dimension_cm3 > 0) {
        calculated_fare = (fare_by_weight > fare_by_dimension) ? fare_by_weight : fare_by_dimension;
    } else {
        calculated_fare = fare_by_weight;
    }

    generate_tracking_number(tracking_num, sizeof(tracking_num));

    printf("\n===========================================\n");
    printf("             DETAIL RESI KARGO             \n");
    printf("===========================================\n");
    printf("Nomor Resi        : %s\n", tracking_num);
    printf("Asal              : %s\n", selected_route->origin);
    printf("Tujuan            : %s\n", selected_route->destination);
    printf("Berat Barang      : %.2f kg\n", weight_kg);
    if (dimension_cm3 > 0) {
        printf("Dimensi Barang    : %.0f cm x %.0f cm x %.0f cm (Volume: %.0f cm³)\n",
               length_cm, width_cm, height_cm, dimension_cm3);
    } else {
        printf("Dimensi Barang    : Tidak Ada (Ongkos dihitung berdasarkan berat)\n");
    }
    printf("Total Ongkos Kirim: Rp. %.2f\n", calculated_fare);
    printf("===========================================\n");

    return 0;
}
