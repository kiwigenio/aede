import json
from collections import defaultdict
import math

archivo_json = "arequipa_graph.json"
archivo_h = "grafo_arequipa.h"

# Leer JSON
with open(archivo_json, "r", encoding="utf-8") as f:
    data = json.load(f)

nodes = data["nodes"]
edges = data["edges"]

# Calcular grado
grado = defaultdict(int)
for edge in edges:
    grado[edge["from"]] += 1
    grado[edge["to"]] += 1

# Filtrar nodos con grado >= 2
nodos_validos_ids = {nid for nid, g in grado.items() if g >= 2}
nodos_filtrados = [n for n in nodes if n["id"] in nodos_validos_ids]
id_to_index = {n["id"]: i for i, n in enumerate(nodos_filtrados)}
NODE_COUNT = len(nodos_filtrados)

# Filtrar aristas
edges_filtradas = [
    e for e in edges
    if e["from"] in nodos_validos_ids and e["to"] in nodos_validos_ids
]

# Crear listas de adyacencia y pesos
adj = [[] for _ in range(NODE_COUNT)]
weight = [[] for _ in range(NODE_COUNT)]

for edge in edges_filtradas:
    u = id_to_index[edge["from"]]
    v = id_to_index[edge["to"]]
    x1, y1 = nodes[id_to_index[edge["from"]]]["lon"], nodes[id_to_index[edge["from"]]]["lat"]
    x2, y2 = nodes[id_to_index[edge["to"]]]["lon"], nodes[id_to_index[edge["to"]]]["lat"]
    dist = math.hypot(x2 - x1, y2 - y1)

    adj[u].append(v)
    weight[u].append(dist)
    adj[v].append(u)
    weight[v].append(dist)

# OFFSET, NEIGHBOR, WEIGHT
OFFSET = [0]
NEIGHBOR = []
WEIGHT = []

for vecinos, pesos in zip(adj, weight):
    ordenados = sorted(zip(vecinos, pesos))
    for v, w in ordenados:
        NEIGHBOR.append(v)
        WEIGHT.append(w)
    OFFSET.append(len(NEIGHBOR))

# Coordenadas escaladas
lon_min = min(n["lon"] for n in nodos_filtrados)
lon_max = max(n["lon"] for n in nodos_filtrados)
lat_min = min(n["lat"] for n in nodos_filtrados)
lat_max = max(n["lat"] for n in nodos_filtrados)

POS_X = [((n["lon"] - lon_min) / (lon_max - lon_min)) * 1200 for n in nodos_filtrados]
POS_Y = [((lat_max - n["lat"]) / (lat_max - lat_min)) * 800 for n in nodos_filtrados]

# === Exportar .h ===
with open(archivo_h, "w", encoding="utf-8") as f:
    f.write("#pragma once\n\n")
    f.write(f"constexpr int NODE_COUNT = {NODE_COUNT};\n")
    f.write(f"constexpr int EDGE_COUNT = {len(NEIGHBOR)};\n\n")

    f.write("constexpr int OFFSET[NODE_COUNT + 1] = {\n    ")
    f.write(", ".join(map(str, OFFSET)))
    f.write("\n};\n\n")

    f.write("constexpr int NEIGHBOR[EDGE_COUNT] = {\n    ")
    f.write(", ".join(map(str, NEIGHBOR)))
    f.write("\n};\n\n")

    f.write("constexpr double WEIGHT[EDGE_COUNT] = {\n    ")
    f.write(", ".join(f"{w:.6f}" for w in WEIGHT))
    f.write("\n};\n\n")

    f.write("constexpr float POS_X[NODE_COUNT] = {\n    ")
    f.write(", ".join(f"{x:.6f}" for x in POS_X))
    f.write("\n};\n\n")

    f.write("constexpr float POS_Y[NODE_COUNT] = {\n    ")
    f.write(", ".join(f"{y:.6f}" for y in POS_Y))
    f.write("\n};\n")

print(f"✅ Archivo generado con pesos: {archivo_h}")
