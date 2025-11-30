#!/bin/bash

# Script para comparar evapotranspiration_24h.tif entre versões kernels e serial
# Regras:
# - Apenas kernels vs serial (não kernels vs kernels)
# - SEBAL só compara com SEBAL
# - STEEP só compara com STEEP

# Criar diretório de resultados se não existir
mkdir -p ./results

echo "=========================================="
echo "Iniciando comparações entre kernels e serial"
echo "=========================================="
echo ""

# Comparações SEBAL
echo "=========================================="
echo "COMPARAÇÕES SEBAL"
echo "=========================================="
echo ""

echo "1. Comparando kernels-double-fm-r-sebal vs serial-double-r-sebal"
make exec-eval \
  EVAL_TIFF_1=./input/serial-double-r-sebal/evapotranspiration_24h.tif \
  EVAL_TIFF_2=./input/kernels-double-fm-r-sebal/evapotranspiration_24h.tif \
  EVAL_OUTPUT_DIR=./results/serial-double_kernels-double-fm-r_SEBAL.txt
echo ""

echo "2. Comparando kernels-double-fm-s-sebal vs serial-double-r-sebal"
make exec-eval \
  EVAL_TIFF_1=./input/serial-double-r-sebal/evapotranspiration_24h.tif \
  EVAL_TIFF_2=./input/kernels-double-fm-s-sebal/evapotranspiration_24h.tif \
  EVAL_OUTPUT_DIR=./results/serial-double_kernels-double-fm-s_SEBAL.txt
echo ""

echo "3. Comparando kernels-float-fm-s-sebal vs serial-double-r-sebal"
make exec-eval \
  EVAL_TIFF_1=./input/serial-double-r-sebal/evapotranspiration_24h.tif \
  EVAL_TIFF_2=./input/kernels-float-fm-s-sebal/evapotranspiration_24h.tif \
  EVAL_OUTPUT_DIR=./results/serial-double_kernels-float-fm-s_SEBAL.txt
echo ""

# Comparações STEEP
echo "=========================================="
echo "COMPARAÇÕES STEEP"
echo "=========================================="
echo ""

echo "4. Comparando kernels-double-fm-r-steep vs serial-double-r-steep"
make exec-eval \
  EVAL_TIFF_1=./input/serial-double-r-steep/evapotranspiration_24h.tif \
  EVAL_TIFF_2=./input/kernels-double-fm-r-steep/evapotranspiration_24h.tif \
  EVAL_OUTPUT_DIR=./results/serial-double_kernels-double-fm-r_STEEP.txt
echo ""

echo "5. Comparando kernels-double-fm-s-steep vs serial-double-r-steep"
make exec-eval \
  EVAL_TIFF_1=./input/serial-double-r-steep/evapotranspiration_24h.tif \
  EVAL_TIFF_2=./input/kernels-double-fm-s-steep/evapotranspiration_24h.tif \
  EVAL_OUTPUT_DIR=./results/serial-double_kernels-double-fm-s_STEEP.txt
echo ""

echo "6. Comparando kernels-float-fm-s-steep vs serial-double-r-steep"
make exec-eval \
  EVAL_TIFF_1=./input/serial-double-r-steep/evapotranspiration_24h.tif \
  EVAL_TIFF_2=./input/kernels-float-fm-s-steep/evapotranspiration_24h.tif \
  EVAL_OUTPUT_DIR=./results/serial-double_kernels-float-fm-s_STEEP.txt
echo ""

echo "=========================================="
echo "Todas as comparações foram concluídas!"
echo "Resultados salvos em ./results/"
echo "=========================================="
echo ""
echo "Arquivos gerados:"
ls -lh ./results/*.txt

