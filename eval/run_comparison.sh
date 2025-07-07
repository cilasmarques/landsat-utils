#!/bin/bash

# Script para instalar dependências e executar comparação de TIFFs

echo "Instalando dependências..."
pip install -r requirements.txt

echo "Executando comparação de TIFFs..."
python compare_tiffs.py

echo "Comparação concluída! Verifique os arquivos CSV gerados." 