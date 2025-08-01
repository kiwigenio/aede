# Script de compilación para el proyecto de rutas
# Requiere tener instalado g++ y SFML

Write-Host "Compilando proyecto de rutas..." -ForegroundColor Green

# Compilar todos los archivos .cpp
$sources = @(
    "mapa_grafo.cpp",
    "bfs.cpp", 
    "dfs.cpp",
    "best_first_search.cpp",
    "a_estrella.cpp",
    "dijkstra.cpp"
)

$output = "mapa_arequipa.exe"

# Comando de compilación con rutas locales de SFML
$command = "g++ -std=c++17 -O2 -I`"SFML/include`" " + ($sources -join " ") + " -o $output -L`"SFML/lib`" -lsfml-graphics -lsfml-window -lsfml-system"

Write-Host "Ejecutando: $command" -ForegroundColor Yellow

try {
    Invoke-Expression $command
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Compilación exitosa!" -ForegroundColor Green
        Write-Host "Ejecutar con: ./$output" -ForegroundColor Cyan
        Write-Host ""
        Write-Host "Controles:" -ForegroundColor Yellow
        Write-Host "- Click izquierdo: Seleccionar nodo origen/destino" -ForegroundColor White
        Write-Host "- Click derecho + arrastrar: Mover vista" -ForegroundColor White  
        Write-Host "- Scroll: Zoom" -ForegroundColor White
        Write-Host "- B: BFS" -ForegroundColor White
        Write-Host "- D: DFS" -ForegroundColor White
        Write-Host "- G: Best First Search" -ForegroundColor White
        Write-Host "- J: Dijkstra" -ForegroundColor White
        Write-Host "- A: A*" -ForegroundColor White
        Write-Host "- R: Reset selección" -ForegroundColor White
    } else {
        Write-Host "✗ Error en la compilación" -ForegroundColor Red
    }
} catch {
    Write-Host "✗ Error: $_" -ForegroundColor Red
    Write-Host ""
    Write-Host "Nota: Asegúrate de tener instalado:" -ForegroundColor Yellow
    Write-Host "- MinGW-w64 (g++)" -ForegroundColor White
    Write-Host "- SFML development libraries" -ForegroundColor White
}
