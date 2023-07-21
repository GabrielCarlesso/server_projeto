#include <stdbool.h>

struct Point {
    double lat; // latitude
    double lon; // longitude
};

 struct Point polygon[] = { {-27.863127802787478, -54.47114119114327},
                            {-27.862207562732074, -54.466914454584234},
                            {-27.870656746169345, -54.469516736427934},           
                            {-27.866146285430734, -54.47461677133878}
    };


// Função para verificar se o ponto está dentro do polígono
int isInsidePolygon(struct Point point, struct Point polygon[], int vertices) {
    int intersectCount = 0;

    for (int i = 0; i < vertices; i++) {
        struct Point p1 = polygon[i];
        struct Point p2 = polygon[(i + 1) % vertices];

        // Verificar se a reta horizontal que passa pelo ponto cruza o segmento do polígono
        if ((p1.lat > point.lat) != (p2.lat > point.lat)) {
            double intersectLon = (p2.lon - p1.lon) * (point.lat - p1.lat) / (p2.lat - p1.lat) + p1.lon;
            
            // Verificar se a interseção está à direita do ponto
            if (point.lon < intersectLon)
                intersectCount++;
        }
    }

    // O ponto está dentro do polígono se o número de interseções for ímpar
    return (intersectCount % 2 == 1);
}
