#!/bin/bash
#SBATCH --job-name=kmeansOpenMP
#SBATCH --mail-type=ALL
#SBATCH --mail-user=davide.pozzoli@studio.unibo.it
#SBATCH --time=02:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#SBATCH --output=outputlog
#SBATCH --gres=gpu:1

./launch_house
