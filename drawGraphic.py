import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter
import argparse
import sys
import os

def main():
    parser = argparse.ArgumentParser(description='Визуализация данных с высоким масштабом.')
    parser.add_argument('file', help='Путь к CSV файлу')
    parser.add_argument('--x', help='Имя столбца X', default=None)
    parser.add_argument('--y', help='Имя столбца Y', default=None)
    parser.add_argument('--output', '-o', help='Путь для сохранения .png', default=None)
    
    args = parser.parse_args()

    try:
        # Читаем файл с автоопределением разделителя (запятая или точка с запятой)
        df = pd.read_csv(args.file, sep=None, engine='python')
        
        if df.shape[1] < 2:
            print(f"Ошибка: В файле найден только один столбец. Проверьте формат.")
            sys.exit(1)

        x_col = args.x if args.x else df.columns[0]
        y_col = args.y if args.y else df.columns[1]

        # Настройка графического окна
        fig, ax = plt.subplots(figsize=(12, 7))

        # Рисуем график
        # Если точек слишком много, можно добавить децимацию: df[::10]
        ax.plot(df[x_col], df[y_col], color='royalblue', linewidth=1.5, label='Данные')

        # --- НАСТРОЙКА МАСШТАБА И ФОРМАТА ---
        
        # Отключаем "научную нотацию" (например, 1e7)
        formatter = ScalarFormatter(useOffset=False)
        formatter.set_scientific(False)
        ax.xaxis.set_major_formatter(formatter)
        ax.yaxis.set_major_formatter(formatter)

        # Если это координаты Мандельброта, полезно включить axis('equal')
        # ax.axis('equal') 

        # Сетка и подписи
        ax.set_title(f"График: {y_col} vs {x_col}", fontsize=14, pad=15)
        ax.set_xlabel(x_col, fontsize=12)
        ax.set_ylabel(y_col, fontsize=12)
        ax.grid(True, linestyle='--', alpha=0.6)
        
        # Поворачиваем метки X, если числа очень длинные
        plt.xticks(rotation=45)
        plt.tight_layout()

        # Сохранение
        output_path = "image" + args.output if args.output else os.path.splitext(args.file)[0] + ".png"
        plt.savefig(output_path, dpi=300)
        print(f"✅ График сохранен в: {output_path}")

        # Показ окна
        # plt.show()

    except FileNotFoundError:
        print(f"Ошибка: Файл '{args.file}' не найден.")
    except Exception as e:
        print(f"Произошла ошибка: {e}")

if __name__ == "__main__":
    main()