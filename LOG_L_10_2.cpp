#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <random>
#include <locale>

int_fast8_t** create_graph(int size);
void initialize_unorient_graph(int size, int_fast8_t** graph_pointer, float edge_probability);
void initialize_orient_graph(int size, int_fast8_t** graph_pointer, float edge_probability);
void print_graph(int size, int_fast8_t** graph_pointer);
void floyd_warshall(int size, int_fast8_t** graph, int** distances);
void find_radius_and_diameter(int size, int_fast8_t** graph, int* radius, int* diameter);
void find_peripheral_vertices(int size, int_fast8_t** graph, int diameter);
void find_central_vertices(int size, int_fast8_t** graph, int radius);

int main(void) {
	setlocale(LC_ALL, "Russian");
	int_fast8_t** graph1;
	int_fast8_t** graph2;
	int size1, size2;
	int radius1, diameter1, radius2, diameter2;
	float edge_probability1, edge_probability2;

	fprintf(stdout, "Введите количество вершин неориентированного графа: ");
	if (!scanf("%d", &size1) || size1 <= 1) {
		fprintf(stderr, "Ошибка ввода количества вершин");
		exit(1);
	}

	graph1 = create_graph(size1);

	fprintf(stdout, "Введите вероятность создания ребра в неориентированном графе (от 0 до 100): ");
	if (!scanf("%f", &edge_probability1) || edge_probability1 < 0 || edge_probability1 > 100) {
		fprintf(stderr, "Ошибка ввода вероятности");
		exit(1);
	}

	srand((unsigned int)time(NULL));
	initialize_unorient_graph(size1, graph1, edge_probability1);

	fprintf(stdout, "Введите количество вершин ориентированного графа: ");
	if (!scanf("%d", &size2) || size2 <= 1) {
		fprintf(stderr, "Ошибка ввода количества вершин");
		exit(1);
	}

	graph2 = create_graph(size2);

	fprintf(stdout, "Введите вероятность создания ребра в ориентированном графе (от 0 до 100): ");
	if (!scanf("%f", &edge_probability2) || edge_probability2 < 0 || edge_probability2 > 100) {
		fprintf(stderr, "Ошибка ввода вероятности");
		exit(1);
	}

	srand((unsigned int)time(NULL));
	initialize_orient_graph(size2, graph2, edge_probability2);
	printf("\nМатрица смежности неориентированного графа: \n\n");
	print_graph(size1, graph1);
	printf("\nМатрица смежности ориентированного графа: \n\n");
	print_graph(size2, graph2);

	find_radius_and_diameter(size1, graph1, &radius1, &diameter1);
	find_radius_and_diameter(size2, graph2, &radius2, &diameter2);

	printf("\nНеориентированный граф:\n");
	printf("Радиус: %d\n", radius1);
	printf("Диаметр: %d\n", diameter1);
	find_peripheral_vertices(size1, graph1, diameter1);
	find_central_vertices(size1, graph1, radius1);

	printf("\nОриентированный граф:\n");
	printf("Радиус: %d\n", radius2);
	printf("Диаметр: %d\n", diameter2);
	find_peripheral_vertices(size2, graph2, diameter2);
	find_central_vertices(size2, graph2, radius2);

	for (int i = 0; i < size1; i++) {
		free(graph1[i]);
	}
	free(graph1);

	for (int i = 0; i < size2; i++) {
		free(graph2[i]);
	}
	free(graph2);

	return 0;
}

int_fast8_t** create_graph(int size) {
	int_fast8_t** array = (int_fast8_t**)malloc(sizeof(int_fast8_t*) * size);
	for (int i = 0; i < size; i++) {
		array[i] = (int_fast8_t*)malloc(sizeof(int_fast8_t) * size);
	}
	if (array == NULL) {
		fprintf(stderr, "Ошибка создания массива");
		exit(1);
	}
	return array;
}

void initialize_unorient_graph(int size, int_fast8_t** graph_pointer, float edge_probability) {
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if ((float)rand() / (RAND_MAX / 100) < edge_probability) {
				int distanse = (int)rand() % 9 + 1;
				graph_pointer[i][j] = distanse;
				graph_pointer[j][i] = distanse;
			}
			else {
				graph_pointer[i][j] = 0;
				graph_pointer[j][i] = 0;
			}
		}
		graph_pointer[i][i] = 0;
	}
}

void initialize_orient_graph(int size, int_fast8_t** graph_pointer, float edge_probability) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if ((float)rand() / (RAND_MAX / 100) < edge_probability) {
				graph_pointer[i][j] = (int)rand() % 9 + 1;
			}
			else {
				graph_pointer[i][j] = 0;
			}
		}
		graph_pointer[i][i] = 0;
	}
}

void print_graph(int size, int_fast8_t** graph_pointer) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fprintf(stdout, "%d ", graph_pointer[j][i]);
		}
		fprintf(stdout, "\n");
	}
}

void floyd_warshall(int size, int_fast8_t** graph, int** distances) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == j)
				distances[i][j] = 0;
			else if (graph[i][j] != 0)
				distances[i][j] = graph[i][j];
			else
				distances[i][j] = INT_MAX;
		}
	}

	for (int k = 0; k < size; k++) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (distances[i][k] != INT_MAX && distances[k][j] != INT_MAX &&
					distances[i][k] + distances[k][j] < distances[i][j]) {
					distances[i][j] = distances[i][k] + distances[k][j];
				}
			}
		}
	}
}

int eccentricity(int vertex, int size, int** distances) {
	int max_distance = 0;
	for (int i = 0; i < size; i++) {
		if (distances[vertex][i] != INT_MAX && distances[vertex][i] > max_distance) {
			max_distance = distances[vertex][i];
		}
	}
	return max_distance;
}

void find_radius_and_diameter(int size, int_fast8_t** graph, int* radius, int* diameter) {
	int** dist_matrix = (int**)malloc(sizeof(int*) * size);
	for (int i = 0; i < size; i++) {
		dist_matrix[i] = (int*)malloc(sizeof(int) * size);
	}

	floyd_warshall(size, graph, dist_matrix);

	*radius = INT_MAX;
	*diameter = 0;

	printf("\nЭксцентриситеты вершин: ");
	for (int i = 0; i < size; i++) {
		int ecc2 = eccentricity(i, size, dist_matrix);
		printf("%d ", ecc2);
	}
	printf("\n");

	for (int i = 0; i < size; i++) {
		int ecc = eccentricity(i, size, dist_matrix);
		if (ecc != 0) {
			if (ecc < *radius) *radius = ecc;
			if (ecc > *diameter) *diameter = ecc;
		}
	}

	for (int i = 0; i < size; i++) {
		free(dist_matrix[i]);
	}
	free(dist_matrix);
}

void find_peripheral_vertices(int size, int_fast8_t** graph, int diameter) {
	int** dist_matrix = (int**)malloc(sizeof(int*) * size);
	for (int i = 0; i < size; i++) {
		dist_matrix[i] = (int*)malloc(sizeof(int) * size);
	}

	floyd_warshall(size, graph, dist_matrix);

	printf("Периферийные вершины: ");
	for (int i = 0; i < size; i++) {
		int eccentr = eccentricity(i, size, dist_matrix);
		if (eccentr == diameter) {
			printf("%d ", i + 1);
		}
	}
	printf("\n");

	for (int i = 0; i < size; i++) {
		free(dist_matrix[i]);
	}
	free(dist_matrix);
}

void find_central_vertices(int size, int_fast8_t** graph, int radius) {
	int** dist_matrix = (int**)malloc(sizeof(int*) * size);
	for (int i = 0; i < size; i++) {
		dist_matrix[i] = (int*)malloc(sizeof(int) * size);
	}

	floyd_warshall(size, graph, dist_matrix);

	printf("Центральные вершины: ");
	for (int i = 0; i < size; i++) {
		int eccentr = eccentricity(i, size, dist_matrix);
		if (eccentr == radius) {
			printf("%d ", i + 1);
		}
	}
	printf("\n");

	for (int i = 0; i < size; i++) {
		free(dist_matrix[i]);
	}
	free(dist_matrix);
}