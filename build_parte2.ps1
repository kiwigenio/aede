# Script de compilación para Parte II - Grafos Grandes
# Optimizado para rendimiento y paralelismo

Write-Host "=== COMPILANDO PARTE II: GRAFOS GRANDES ===" -ForegroundColor Green

# Archivos fuente para Parte II
$sources_parte2 = @(
    "parte2_main.cpp",
    "grafo_grande.cpp", 
    "algoritmos_grandes.cpp",
    "dijkstra_grande.cpp",
    "metricas.cpp"
)

$output_parte2 = "parte2_benchmark.exe"

Write-Host "Compilando con optimizaciones para rendimiento..." -ForegroundColor Yellow

# Compilación optimizada para Parte II
$flags_optimizacion = @(
    "-std=c++17",
    "-O2",                    # Optimización nivel 2 (más compatible)
    "-DNDEBUG"               # Desactivar asserts
)

$command_parte2 = "g++ " + ($flags_optimizacion -join " ") + " " + ($sources_parte2 -join " ") + " -o $output_parte2"

Write-Host "Ejecutando: $command_parte2" -ForegroundColor Cyan

try {
    Invoke-Expression $command_parte2
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Compilación exitosa!" -ForegroundColor Green
        Write-Host ""
        Write-Host "=== PARTE II LISTA PARA EJECUTAR ===" -ForegroundColor Yellow
        Write-Host "Ejecutar con: ./$output_parte2" -ForegroundColor White
        Write-Host ""
        Write-Host "Caracteristicas:" -ForegroundColor Yellow
        Write-Host "- Grafo sintetico de 2M nodos" -ForegroundColor White
        Write-Host "- Pruebas en paralelo con todos los cores" -ForegroundColor White
        Write-Host "- Metricas detalladas de rendimiento" -ForegroundColor White
        Write-Host "- Exportacion a CSV y HTML" -ForegroundColor White
        Write-Host "- Optimizaciones de compilador activadas" -ForegroundColor White
        Write-Host ""
        Write-Host "Algoritmos incluidos:" -ForegroundColor Yellow
        Write-Host "- BFS, DFS, Best First Search" -ForegroundColor White
        Write-Host "- Dijkstra (optimizado)" -ForegroundColor White
        Write-Host "- A* (con heuristica)" -ForegroundColor White
        
        # Mostrar información del sistema
        Write-Host ""
        Write-Host "Información del sistema:" -ForegroundColor Yellow
        $cores = [Environment]::ProcessorCount
        Write-Host "- CPU Cores: $cores" -ForegroundColor White
        $memoria = [Math]::Round((Get-CimInstance Win32_ComputerSystem).TotalPhysicalMemory / 1GB, 1)
        Write-Host "- RAM Total: $memoria GB" -ForegroundColor White
        
        Write-Host ""
        Write-Host "ADVERTENCIA: El programa usara aproximadamente 1-2 GB de RAM" -ForegroundColor Red
        Write-Host "Tiempo estimado de ejecucion: 5-15 minutos" -ForegroundColor Red
        
    } else {
        Write-Host "✗ Error en la compilación" -ForegroundColor Red
    }
} catch {
    Write-Host "✗ Error: $_" -ForegroundColor Red
    Write-Host ""
    Write-Host "Nota: Para Parte II se requiere:" -ForegroundColor Yellow
    Write-Host "- MinGW-w64 con soporte OpenMP" -ForegroundColor White
    Write-Host "- Al menos 4 GB de RAM disponible" -ForegroundColor White
    Write-Host "- CPU con multiples cores (recomendado)" -ForegroundColor White
}

Write-Host ""
Write-Host "Para compilar Parte I (con visualizacion):" -ForegroundColor Cyan
Write-Host ".\build.ps1" -ForegroundColor White
