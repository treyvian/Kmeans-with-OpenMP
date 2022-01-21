#!/bin/bash
#SBATCH --job-name=kmeansOpenMP
#SBATCH --mail-type=ALL
#SBATCH --mail-user=davide.pozzoli@studio.unibo.it
#SBATCH --time=01:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --output=nomeoutput
#SBATCH --gres=gpu:1

./launch_mall
sh launch_house.sh