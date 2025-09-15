#!/usr/bin/env python3
import argparse
from PIL import Image
import numpy as np
import subprocess
import tempfile
import os

def calculate_ssim_simple(img1, img2):
    """
    Calcula uma versão simplificada do SSIM sem usar scikit-image
    """
    # As imagens já devem estar convertidas para float e arredondadas
    # Converter para float caso não esteja
    img1 = img1.astype(np.float64)
    img2 = img2.astype(np.float64)
    
    # Parâmetros do SSIM
    C1 = (0.01 * 255) ** 2
    C2 = (0.03 * 255) ** 2
    
    # Médias
    mu1 = np.mean(img1)
    mu2 = np.mean(img2)
    
    # Variâncias e covariância
    var1 = np.var(img1)
    var2 = np.var(img2)
    covar = np.mean((img1 - mu1) * (img2 - mu2))
    
    # SSIM
    numerator = (2 * mu1 * mu2 + C1) * (2 * covar + C2)
    denominator = (mu1**2 + mu2**2 + C1) * (var1 + var2 + C2)
    
    ssim = numerator / denominator
    return ssim

def main():
    # Configurar argumentos de linha de comando
    parser = argparse.ArgumentParser(description='Calcula métricas de similaridade entre duas imagens TIFF')
    parser.add_argument('img1', help='Caminho para a primeira imagem TIFF')
    parser.add_argument('img2', help='Caminho para a segunda imagem TIFF')
    
    args = parser.parse_args()
    
    try:
        # Função para converter arquivo para 32-bit se necessário
        def convert_to_32bit_if_needed(filepath):
            try:
                # Tentar abrir diretamente com PIL
                img = Image.open(filepath)
                return np.array(img.convert("L"))
            except:
                # Se falhar, converter para 32-bit usando ImageMagick
                with tempfile.NamedTemporaryFile(suffix='.tif', delete=False) as tmp_file:
                    tmp_path = tmp_file.name
                
                try:
                    # Tentar converter preservando a escala original
                    subprocess.run([
                        'convert', filepath, '-depth', '32', '-define', 'tiff:compression=lzw', 
                        '-define', 'quantum:format=floating-point', '-define', 'quantum:scale=1', tmp_path
                    ], check=True, capture_output=True)
                    
                    # Abrir o arquivo convertido
                    img = Image.open(tmp_path)
                    result = np.array(img.convert("L"))
                    
                    # Limpar arquivo temporário
                    os.unlink(tmp_path)
                    return result
                except:
                    # Se ImageMagick falhar, tentar abrir como array numpy
                    from osgeo import gdal
                    dataset = gdal.Open(filepath)
                    if dataset is not None:
                        band = dataset.GetRasterBand(1)
                        result = band.ReadAsArray().astype(np.float32)
                        dataset = None
                        return result
                    else:
                        raise ValueError(f"Não foi possível abrir o arquivo: {filepath}")
        
        # Abrir as imagens
        img1 = convert_to_32bit_if_needed(args.img1)
        img2 = convert_to_32bit_if_needed(args.img2)
        
        # Verifica se têm o mesmo tamanho
        if img1.shape != img2.shape:
            raise ValueError("As imagens precisam ter o mesmo tamanho")
        
        # Normalizar ambas as imagens para a mesma escala (0-1) antes de arredondar
        img1_norm = (img1.astype(np.float64) - np.min(img1)) / (np.max(img1) - np.min(img1) + 1e-10)
        img2_norm = (img2.astype(np.float64) - np.min(img2)) / (np.max(img2) - np.min(img2) + 1e-10)
        
        # Arredondar para 3 casas decimais após normalização
        img1_rounded = np.round(img1_norm, 3)
        img2_rounded = np.round(img2_norm, 3)
        
        # Calcular métricas com valores arredondados
        ssim_simple = calculate_ssim_simple(img1_rounded, img2_rounded)
        mae = np.mean(np.abs(img1_rounded - img2_rounded))
        mse = np.mean((img1_rounded - img2_rounded) ** 2)
        
        # Correlação de Pearson
        correlation = np.corrcoef(img1_rounded.flatten(), img2_rounded.flatten())[0, 1]
        
        # Diferença percentual média
        nonzero_mask = img1_rounded != 0
        if np.any(nonzero_mask):
            mean_percent_diff = np.mean(np.abs((img1_rounded[nonzero_mask] - img2_rounded[nonzero_mask]) / img1_rounded[nonzero_mask])) * 100
        else:
            mean_percent_diff = 0

        # Mostrar valor de um pixel específico para debug
        row, col = 4866, 3768
        if row < img1.shape[0] and col < img1.shape[1]:
            pixel1_original = img1_rounded[row, col]
            pixel2_original = img2_rounded[row, col]
            print(f"Pixel [{row},{col}] - Imagem 1: {pixel1_original:.6f}, Imagem 2: {pixel2_original:.6f}")
            print(f"Diferença absoluta: {abs(pixel1_original - pixel2_original):.6f}")
            if pixel1_original != 0:
                print(f"Diferença percentual: {abs(pixel1_original - pixel2_original) / abs(pixel1_original) * 100:.6f}%")
        else:
            print(f"Pixel [{row},{col}] está fora dos limites das imagens")
        
        # Mostrar estatísticas das imagens
        print(f"\nEstatísticas das imagens:")
        print(f"Imagem 1 - Min: {np.min(img1_rounded):.6f}, Max: {np.max(img1_rounded):.6f}, Média: {np.mean(img1_rounded):.6f}")
        print(f"Imagem 2 - Min: {np.min(img2_rounded):.6f}, Max: {np.max(img2_rounded):.6f}, Média: {np.mean(img2_rounded):.6f}")
        
        # Encontrar pixels com diferenças significativas
        diff_mask = np.abs(img1_rounded - img2_rounded) > 0.001
        if np.any(diff_mask):
            diff_indices = np.where(diff_mask)
            print(f"\nPrimeiros 5 pixels com diferença > 0.001:")
            for k in range(min(5, len(diff_indices[0]))):
                i, j = diff_indices[0][k], diff_indices[1][k]
                print(f"Pixel [{i},{j}] - Imagem 1: {img1_rounded[i, j]:.6f}, Imagem 2: {img2_rounded[i, j]:.6f}, Diff: {abs(img1_rounded[i, j] - img2_rounded[i, j]):.6f}")
        else:
            print("\nNenhum pixel com diferença > 0.001 encontrado")
        
        print("Comparação considerando valores arredondados para 3 casas decimais")
        print(f"SSIM (simplificado): {ssim_simple:.6f}")
        print(f"MAE: {mae:.6f}")
        print(f"MSE: {mse:.6f}")
        print(f"RMSE: {np.sqrt(mse):.6f}")
        print(f"Correlação: {correlation:.6f}")
        print(f"Diff. % média: {mean_percent_diff:.6f}%")

    except FileNotFoundError as e:
        print(f"Erro: Arquivo não encontrado - {e}")
    except Exception as e:
        print(f"Erro: {e}")

if __name__ == "__main__":
    main() 