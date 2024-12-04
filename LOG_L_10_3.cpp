#define _CRT_SECURE_NO_WARNINGS
#define Size_graph 6
#define Edge_probability 50

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <malloc.h>
#include <random>
#include <locale>
#include <string.h>

int_fast8_t** create_graph(int size);
void initialize_unweighted_unorient_graph(int size, int_fast8_t** graph_pointer, float edge_probability);
void initialize_unweighted_orient_graph(int size, int_fast8_t** graph_pointer, float edge_probability);
void initialize_weighted_unorient_graph(int size, int_fast8_t** graph_pointer, float edge_probability);
void initialize_weighted_orient_graph(int size, int_fast8_t** graph_pointer, float edge_probability);
void print_graph(int size, int_fast8_t** graph_pointer);
void print_guide();
void floyd_warshall(int size, int_fast8_t** graph, int** distances);
void find_radius_and_diameter(int size, int_fast8_t** graph, int* radius, int* diameter);
void find_peripheral_vertices(int size, int_fast8_t** graph, int diameter);
void find_central_vertices(int size, int_fast8_t** graph, int radius);
int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	int_fast8_t** graph;
	bool is_weighted = false;
	bool is_oriented = false;

	if (argc != 5) {
		print_guide();
		int rofls;
		scanf("%d", &rofls);
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-w") == 0) {
			if (strcmp(argv[i + 1], "yes") == 0) {
				is_weighted = true;
			}
			else if (strcmp(argv[i + 1], "no") == 0) {
				is_weighted = false;
			}
			else {
				print_guide();
				return 1;
			}
		}
		else if (strcmp(argv[i], "-o") == 0) {
			if (strcmp(argv[i + 1], "yes") == 0) {
				is_oriented = true;
			}
			else if (strcmp(argv[i + 1], "no") == 0) {
				is_oriented = false;
			}
			else {
				print_guide();
				return 1;
			}
		}
	}

	graph = create_graph(Size_graph);
	srand((unsigned int)time(NULL));

	if (is_weighted) {
		if (is_oriented) {
			printf("Матрица смежности взвешенного ориентированного графа: \n\n");
			initialize_weighted_orient_graph(Size_graph, graph, Edge_probability);
		}
		else {
			printf("Матрица смежности взвешенного неориентированного графа: \n\n");
			initialize_weighted_unorient_graph(Size_graph, graph, Edge_probability);
		}
	}
	else {
		if (is_oriented) {
			printf("Матрица смежности невзвешенного ориентированного графа: \n\n");
			initialize_unweighted_orient_graph(Size_graph, graph, Edge_probability);
		}
		else {
			printf("Матрица смежности невзвешенного неориентированного графа: \n\n");
			initialize_unweighted_unorient_graph(Size_graph, graph, Edge_probability);
		}
	}

	print_graph(Size_graph, graph);
	printf("\n");
	int radius, diameter;
	find_radius_and_diameter(Size_graph, graph, &radius, &diameter);

	printf("Радиус: %d\n", radius);
	printf("Диаметр: %d\n", diameter);
	find_peripheral_vertices(Size_graph, graph, diameter);
	find_central_vertices(Size_graph, graph, radius);

	int rofls;
	scanf("%d", &rofls);
	for (int i = 0; i < Size_graph; i++) {
		free(graph[i]);
	}
	free(graph);

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

void initialize_unweighted_unorient_graph(int size, int_fast8_t** graph_pointer, float edge_probability) {
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if ((float)rand() / (RAND_MAX / 100) < edge_probability) {
				graph_pointer[i][j] = 1;
				graph_pointer[j][i] = 1;
			}
			else {
				graph_pointer[i][j] = 0;
				graph_pointer[j][i] = 0;
			}
		}
		graph_pointer[i][i] = 0;
	}
}

void initialize_unweighted_orient_graph(int size, int_fast8_t** graph_pointer, float edge_probability) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if ((float)rand() / (RAND_MAX / 100) < edge_probability) {
				graph_pointer[i][j] = 1;
			}
			else {
				graph_pointer[i][j] = 0;
			}
		}
		graph_pointer[i][i] = 0;
	}
}

void initialize_weighted_unorient_graph(int size, int_fast8_t** graph_pointer, float edge_probability) {
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

void initialize_weighted_orient_graph(int size, int_fast8_t** graph_pointer, float edge_probability) {
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

void print_guide() {
	printf("Использование программы:\n");
	printf("program.exe -w [yes|no] -o [yes|no]\n");
	printf("где:\n");
	printf("-w yes|no - взвешенный или невзвешенный граф\n");
	printf("-o yes|no - ориентированный или неориентированный граф\n");
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