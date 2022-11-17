from typing import List

import click
import subprocess
from tqdm import tqdm
import cv2

import matplotlib.pyplot as plt


@click.command()
@click.option('--program-path', help="Path to the executable")
@click.option('--image-path', help="Path to the image")
def main(image_path: str, program_path: str) -> None:
    radii: List[int] = [1, 5, 9, 25, 45, 99]
    algorithms: List[str] = ["simple", "huang", "constant", "opencv"]

    img = cv2.imread(image_path, cv2.IMREAD_UNCHANGED)
    megapixel_count = img.shape[0] * img.shape[1] / 10 ** 6

    results: List[List[int]] = []
    for i, algo in enumerate(algorithms):
        results.append([])

        print(f"Testing {algo}... {i + 1}/{len(algorithms)}")
        for radius in tqdm(radii):
            if algo == "simple" and radius > 10:
                continue

            result = subprocess.run(
                [
                    program_path,
                    "--radius", str(radius),
                    "--filepath", image_path,
                    "--algorithm", algo,
                    "--dont_show_image"
                ],
                capture_output=True
            )
            results[i].append(int(result.stdout) / megapixel_count)

    fig, ax = plt.subplots()
    for i, algo in enumerate(algorithms):
        ax.plot(radii[:len(results[i])], results[i], label=algo)

    ax.legend()
    ax.grid(True)

    plt.title("Performance: Median Filter")
    plt.xlabel("Radius")
    plt.ylabel("Microseconds/Megapixel")

    plt.show()


if __name__ == "__main__":
    main()
