#!/usr/bin/env python3
"""
Script para comparar arquivos TIFF e gerar tabelas de similaridade.
Compara arquivos kernels com seus respectivos arquivos seriais de base.
"""

import os
import numpy as np
import pandas as pd
from rasterio import open as rasterio_open
import argparse
from pathlib import Path


def load_tiff_data(file_path):
    """Carrega dados de um arquivo TIFF."""
    try:
        with rasterio_open(file_path) as src:
            data = src.read(1)  # Lê a primeira banda
            return data
    except Exception as e:
        print(f"Erro ao carregar {file_path}: {e}")
        return None


def compare_arrays(arr1, arr2, precision, method='round'):
    """
    Compara dois arrays com uma precisão específica.
    
    Args:
        arr1: Primeiro array
        arr2: Segundo array
        precision: Precisão para comparação (0.1, 1.0, 5.0)
        method: 'floor' para ignorar casas decimais, 'round' para considerar 2 casas decimais
    
    Returns:
        Número de pixels que divergem na precisão especificada e mediana das diferenças
    """
    if arr1 is None or arr2 is None:
        return None, None, None
    
    # Aplica o método de arredondamento
    if method == 'floor':
        # Ignora casas decimais (arredonda para baixo)
        arr1_rounded = np.floor(arr1 / precision) * precision
        arr2_rounded = np.floor(arr2 / precision) * precision
    else:  # method == 'round'
        # Considera apenas 2 casas decimais
        arr1_rounded = np.round(arr1 / precision, 2) * precision
        arr2_rounded = np.round(arr2 / precision, 2) * precision
    
    # Conta quantos pixels são diferentes
    diff_count = np.sum(arr1_rounded != arr2_rounded)
    total_pixels = arr1.size
    
    # Calcula a mediana das diferenças absolutas entre os arrays originais
    # Remove valores NaN antes de calcular a mediana
    diff_array = np.abs(arr1 - arr2)
    diff_array_clean = diff_array[~np.isnan(diff_array)]
    diff_median = np.median(diff_array_clean) if len(diff_array_clean) > 0 else 0.0
    
    return diff_count, total_pixels, diff_median


def main():
    parser = argparse.ArgumentParser(description='Compara arquivos TIFF e gera tabelas de similaridade')
    parser.add_argument('--tiffs_dir', default='../tiffs', help='Diretório com os arquivos TIFF')
    parser.add_argument('--output_dir', default='.', help='Diretório de saída para os CSVs')
    
    args = parser.parse_args()
    
    tiffs_dir = Path(args.tiffs_dir)
    output_dir = Path(args.output_dir)
    
    # Verifica se o diretório existe
    if not tiffs_dir.exists():
        print(f"Diretório {tiffs_dir} não encontrado!")
        return
    
    # Lista todos os arquivos TIFF
    tiff_files = list(tiffs_dir.glob('*.tif'))
    
    if not tiff_files:
        print(f"Nenhum arquivo TIFF encontrado em {tiffs_dir}")
        return
    
    print(f"Encontrados {len(tiff_files)} arquivos TIFF")
    
    # Organiza os arquivos por tipo
    serial_files = [f for f in tiff_files if 'serial' in f.name]
    kernel_files = [f for f in tiff_files if 'kernels' in f.name]
    
    print(f"Arquivos seriais: {[f.name for f in serial_files]}")
    print(f"Arquivos kernels: {[f.name for f in kernel_files]}")
    
    # Cria dicionário para armazenar resultados
    results = []
    
    # Compara cada arquivo kernel com seu correspondente serial
    for kernel_file in kernel_files:
        # Extrai o número do arquivo (0 ou 1)
        if '-0-' in kernel_file.name:
            serial_number = '0'
        elif '-1-' in kernel_file.name:
            serial_number = '1'
        else:
            print(f"Não foi possível determinar o número do arquivo {kernel_file.name}")
            continue
        
        # Encontra o arquivo serial correspondente
        serial_file = None
        for sf in serial_files:
            if f'-{serial_number}-' in sf.name:
                serial_file = sf
                break
        
        if serial_file is None:
            print(f"Não foi possível encontrar arquivo serial correspondente para {kernel_file.name}")
            continue
        
        print(f"Comparando {kernel_file.name} com {serial_file.name}")
        
        # Carrega os dados
        kernel_data = load_tiff_data(kernel_file)
        serial_data = load_tiff_data(serial_file)
        
        if kernel_data is None or serial_data is None:
            print(f"Erro ao carregar dados para comparação")
            continue
        
        # Comparações com método FLOOR (ignora casas decimais)
        # Precisão 0.1
        result_floor_01 = compare_arrays(kernel_data, serial_data, 0.1, 'floor')
        if result_floor_01[0] is not None:
            diff_count_floor_01, total_pixels, diff_median_floor_01 = result_floor_01
            similarity_floor_01 = ((total_pixels - diff_count_floor_01) / total_pixels) * 100 if total_pixels > 0 else 0
        else:
            diff_count_floor_01, total_pixels, diff_median_floor_01 = 0, 0, 0
            similarity_floor_01 = 0

        # Precisão 1.0
        result_floor_1 = compare_arrays(kernel_data, serial_data, 1.0, 'floor')
        if result_floor_1[0] is not None:
            diff_count_floor_1, _, diff_median_floor_1 = result_floor_1
            similarity_floor_1 = ((total_pixels - diff_count_floor_1) / total_pixels) * 100 if total_pixels > 0 else 0
        else:
            diff_count_floor_1, diff_median_floor_1 = 0, 0
            similarity_floor_1 = 0

        # Precisão 5.0
        result_floor_5 = compare_arrays(kernel_data, serial_data, 5.0, 'floor')
        if result_floor_5[0] is not None:
            diff_count_floor_5, _, diff_median_floor_5 = result_floor_5
            similarity_floor_5 = ((total_pixels - diff_count_floor_5) / total_pixels) * 100 if total_pixels > 0 else 0
        else:
            diff_count_floor_5, diff_median_floor_5 = 0, 0
            similarity_floor_5 = 0

        # Comparações com método ROUND (2 casas decimais)
        # Precisão 0.1
        result_round_01 = compare_arrays(kernel_data, serial_data, 0.1, 'round')
        if result_round_01[0] is not None:
            diff_count_round_01, _, diff_median_round_01 = result_round_01
            similarity_round_01 = ((total_pixels - diff_count_round_01) / total_pixels) * 100 if total_pixels > 0 else 0
        else:
            diff_count_round_01, diff_median_round_01 = 0, 0
            similarity_round_01 = 0

        # Precisão 1.0
        result_round_1 = compare_arrays(kernel_data, serial_data, 1.0, 'round')
        if result_round_1[0] is not None:
            diff_count_round_1, _, diff_median_round_1 = result_round_1
            similarity_round_1 = ((total_pixels - diff_count_round_1) / total_pixels) * 100 if total_pixels > 0 else 0
        else:
            diff_count_round_1, diff_median_round_1 = 0, 0
            similarity_round_1 = 0

        # Precisão 5.0
        result_round_5 = compare_arrays(kernel_data, serial_data, 5.0, 'round')
        if result_round_5[0] is not None:
            diff_count_round_5, _, diff_median_round_5 = result_round_5
            similarity_round_5 = ((total_pixels - diff_count_round_5) / total_pixels) * 100 if total_pixels > 0 else 0
        else:
            diff_count_round_5, diff_median_round_5 = 0, 0
            similarity_round_5 = 0

        # Adiciona resultado
        results.append({
            'kernel_file': kernel_file.name,
            'serial_file': serial_file.name,
            # FLOOR results
            'diff_count_floor_01': diff_count_floor_01,
            'similarity_floor_01': similarity_floor_01,
            'diff_count_floor_1': diff_count_floor_1,
            'similarity_floor_1': similarity_floor_1,
            'diff_count_floor_5': diff_count_floor_5,
            'similarity_floor_5': similarity_floor_5,
            'diff_median_floor_01': diff_median_floor_01,
            'diff_median_floor_1': diff_median_floor_1,
            'diff_median_floor_5': diff_median_floor_5,
            # ROUND results
            'diff_count_round_01': diff_count_round_01,
            'similarity_round_01': similarity_round_01,
            'diff_count_round_1': diff_count_round_1,
            'similarity_round_1': similarity_round_1,
            'diff_count_round_5': diff_count_round_5,
            'similarity_round_5': similarity_round_5,
            'diff_median_round_01': diff_median_round_01,
            'diff_median_round_1': diff_median_round_1,
            'diff_median_round_5': diff_median_round_5,
            'total_pixels': total_pixels
        })
    
    # Cria DataFrame
    df = pd.DataFrame(results)
    
    if df.empty:
        print("Nenhuma comparação foi realizada!")
        return
    
    # Salva resultados - FLOOR (ignora casas decimais)
    output_file_floor_01 = output_dir / 'similaridade_floor_01_decimal.csv'
    output_file_floor_1 = output_dir / 'similaridade_floor_1_unidade.csv'
    output_file_floor_5 = output_dir / 'similaridade_floor_5_unidades.csv'

    # Tabela FLOOR com precisão de 0.1
    df_floor_01 = df[['kernel_file', 'serial_file', 'diff_count_floor_01', 'similarity_floor_01', 'total_pixels', 'diff_median_floor_01']].copy()
    df_floor_01.columns = ['Arquivo_Kernel', 'Arquivo_Serial', 'Pixels_Diferentes', 'Similaridade_%', 'Total_Pixels', 'Diferença_Mediana']
    df_floor_01.to_csv(output_file_floor_01, index=False)

    # Tabela FLOOR com precisão de 1.0
    df_floor_1 = df[['kernel_file', 'serial_file', 'diff_count_floor_1', 'similarity_floor_1', 'total_pixels', 'diff_median_floor_1']].copy()
    df_floor_1.columns = ['Arquivo_Kernel', 'Arquivo_Serial', 'Pixels_Diferentes', 'Similaridade_%', 'Total_Pixels', 'Diferença_Mediana']
    df_floor_1.to_csv(output_file_floor_1, index=False)

    # Tabela FLOOR com precisão de 5.0
    df_floor_5 = df[['kernel_file', 'serial_file', 'diff_count_floor_5', 'similarity_floor_5', 'total_pixels', 'diff_median_floor_5']].copy()
    df_floor_5.columns = ['Arquivo_Kernel', 'Arquivo_Serial', 'Pixels_Diferentes', 'Similaridade_%', 'Total_Pixels', 'Diferença_Mediana']
    df_floor_5.to_csv(output_file_floor_5, index=False)

    # Salva resultados - ROUND (2 casas decimais)
    output_file_round_01 = output_dir / 'similaridade_round_01_decimal.csv'
    output_file_round_1 = output_dir / 'similaridade_round_1_unidade.csv'
    output_file_round_5 = output_dir / 'similaridade_round_5_unidades.csv'

    # Tabela ROUND com precisão de 0.1
    df_round_01 = df[['kernel_file', 'serial_file', 'diff_count_round_01', 'similarity_round_01', 'total_pixels', 'diff_median_round_01']].copy()
    df_round_01.columns = ['Arquivo_Kernel', 'Arquivo_Serial', 'Pixels_Diferentes', 'Similaridade_%', 'Total_Pixels', 'Diferença_Mediana']
    df_round_01.to_csv(output_file_round_01, index=False)

    # Tabela ROUND com precisão de 1.0
    df_round_1 = df[['kernel_file', 'serial_file', 'diff_count_round_1', 'similarity_round_1', 'total_pixels', 'diff_median_round_1']].copy()
    df_round_1.columns = ['Arquivo_Kernel', 'Arquivo_Serial', 'Pixels_Diferentes', 'Similaridade_%', 'Total_Pixels', 'Diferença_Mediana']
    df_round_1.to_csv(output_file_round_1, index=False)

    # Tabela ROUND com precisão de 5.0
    df_round_5 = df[['kernel_file', 'serial_file', 'diff_count_round_5', 'similarity_round_5', 'total_pixels', 'diff_median_round_5']].copy()
    df_round_5.columns = ['Arquivo_Kernel', 'Arquivo_Serial', 'Pixels_Diferentes', 'Similaridade_%', 'Total_Pixels', 'Diferença_Mediana']
    df_round_5.to_csv(output_file_round_5, index=False)
    
    print(f"\nResultados salvos em:")
    print(f"FLOOR (ignora casas decimais):")
    print(f"  - {output_file_floor_01}")
    print(f"  - {output_file_floor_1}")
    print(f"  - {output_file_floor_5}")
    print(f"ROUND (2 casas decimais):")
    print(f"  - {output_file_round_01}")
    print(f"  - {output_file_round_1}")
    print(f"  - {output_file_round_5}")
    
    # Exibe resumo
    print(f"\nResumo das comparações FLOOR (0.1):")
    print(df_floor_01.to_string(index=False))
    print(f"\nResumo das comparações ROUND (0.1):")
    print(df_round_01.to_string(index=False))


if __name__ == "__main__":
    main() 