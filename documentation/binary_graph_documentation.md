# Binarny Format Pliku Grafu

Ten dokument opisuje binarny format pliku używany do serializacji i zapisu struktury Graph

Format binarny zapisywany jest w następującej kolejności:

1. Liczba wierzchołków (int)
2. Liczba krawędzi (int)
3. Wagi wierzchołków (float × `numVertices`)
4. Lista krawędzi (pary int × `numEdges`)

Wszystkie dane zapisywane są w formacie little-endian.

## Struktura Pliku

| Typ         | Nazwa         | Opis                                                       |
|-------------|---------------|------------------------------------------------------------|
| int         | numVertices   | Liczba wierzchołków w grafie                               |
| int         | numEdges      | Liczba krawędzi w grafie                                   |
| float       | weight[i]     | Waga i-tego wierzchołka (indeksowany od 0)                 |
| (int, int)  | edge[i]       | Para (źródło, cel) dla i-tej krawędzi (src[i], dest[i])    |

## Uwagi

- Wierzchołki są indeksowane od `0` do `numVertices - 1`.
- Każda krawędź zapisywana jest jako para liczb całkowitych reprezentujących indeksy wierzchołków (źródłowego i docelowego).