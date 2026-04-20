import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter
import argparse
import sys
import os

def main():
    parser = argparse.ArgumentParser(description='Универсальный визуализатор данных.')
    parser.add_argument('file', help='Путь к CSV файлу')
    parser.add_argument('--x', help='Имя столбца X', default=None)
    parser.add_argument('--y', help='Имя столбца Y', default=None)
    parser.add_argument('--output', '-o', help='Путь для сохранения .png', default=None)
    
    args = parser.parse_args()

    try:
        # 1. Гибкое чтение данных
        # sep=None с engine='python' автоматически определяет запятую, точку с запятой или табуляцию
        df = pd.read_csv(args.file, sep=None, engine='python', skipinitialspace=True)
        
        if df.empty:
            print("Ошибка: Файл пуст.")
            sys.exit(1)

        # 2. Определение столбцов
        # Если не указаны, берем первый и второй. Если столбец один — используем его как Y, а X будет индексом.
        if len(df.columns) == 1:
            y_col = df.columns[0]
            x_col = "Индекс"
            df[x_col] = df.index
        else:
            x_col = args.x if (args.x and args.x in df.columns) else df.columns[0]
            y_col = args.y if (args.y and args.y in df.columns) else df.columns[1]

        # 3. Очистка и конвертация данных
        # Пытаемся превратить всё в числа, ошибки превращаем в NaN, затем удаляем строки с пустотами
        for col in [x_col, y_col]:
            if col in df.columns:
                df[col] = pd.to_numeric(df[col], errors='coerce')
        
        df = df.dropna(subset=[x_col, y_col])

        if df.empty:
            print("Ошибка: После очистки данных не осталось валидных числовых значений.")
            sys.exit(1)

        # 4. Визуализация
        fig, ax = plt.subplots(figsize=(12, 7))

        # Если данных очень много, рисуем точки без линий (scatter-подобно), чтобы не было "каши"
        # Если данных мало, рисуем линию с маркерами
        if len(df) > 5000:
            ax.plot(df[x_col], df[y_col], ',', color='royalblue', alpha=0.5)
        else:
            ax.plot(df[x_col], df[y_col], color='royalblue', linewidth=1.5, marker='o', markersize=3, label='Тренд')

        # 5. Настройка осей и формата
        # Отключаем экспоненциальную запись (чтобы видеть 1000000 вместо 1e6)
        formatter = ScalarFormatter(useOffset=False)
        formatter.set_scientific(False)
        ax.xaxis.set_major_formatter(formatter)
        ax.yaxis.set_major_formatter(formatter)

        # Локализация и подписи
        ax.set_title(f"Распределение данных: {y_col} относительно {x_col}", fontsize=14, pad=15, fontweight='bold')
        ax.set_xlabel(f"Ось: {x_col}", fontsize=12)
        ax.set_ylabel(f"Ось: {y_col}", fontsize=12)
        
        ax.grid(True, linestyle='--', alpha=0.7)
        plt.xticks(rotation=45)
        
        # Автоматический подбор отступов, чтобы подписи не обрезались
        plt.tight_layout()

        # 6. Сохранение
        file_name = os.path.splitext(args.file)[0]
        output_path = args.output if args.output else f"{file_name}_plot.png"
        
        plt.savefig(output_path, dpi=300)
        print(f"✅ Успешно! График сохранен в: {output_path}")
        print(f"📊 Обработано строк: {len(df)}")

    except FileNotFoundError:
        print(f"❌ Ошибка: Файл '{args.file}' не найден.")
    except Exception as e:
        print(f"❌ Произошла непредвиденная ошибка: {e}")

if __name__ == "__main__":
    main()