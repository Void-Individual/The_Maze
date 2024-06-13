#!/usr/bin/env bash
# Script to install SDL2 on a unix system

sudo apt-get update
echo "Installing SDL2"
sudo apt-get install -y libsdl2-dev
echo "Installing SDL2_Image"
sudo apt-get install -y libsdl2-image-dev
