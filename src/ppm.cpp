/*
    Copyright (C) 2009 Johannes Schindelin (johannes.schindelin@gmx.de)

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version
    3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ppm.h"


// FrameExporter

FrameExporter::FrameExporter()
    : surface(NULL), pixels(NULL), rowstride(0) {
}

void FrameExporter::dump() {
    if(!surface) {
        surface = SDL_GetVideoSurface();
	rowstride = surface->w * 3;
	pixels = (char *)malloc((surface->h + 1) * rowstride);
        initialize();
    }
    glReadPixels(0, 0, surface->w, surface->h,
        GL_RGB, GL_UNSIGNED_BYTE, pixels);
    dumpImpl();
}

// PPMExporter

PPMExporter::PPMExporter(std::string outputfile) {

    if(outputfile == "-") {
        output = &std::cout;

    } else {
        filename = outputfile;
        output   = new std::fstream(outputfile.c_str(), std::ios::out | std::ios::binary);

        if(output->fail()) {
            printf("Failed to open '%s' for writing\n", outputfile.c_str());
            exit(1);
        }
    }
}

PPMExporter::~PPMExporter() {
    if(filename.size()>0) ((std::fstream*)output)->close();
}

void PPMExporter::dumpImpl() {
    *output << "P6\n# Generated by Gource\n";
    *output << (surface->w);
    *output << " ";
    *output << (surface->h);
    *output << "\n255\n";

    for(int y = surface->h - 1; y >= 0; y--) {
        output->write(pixels + y * rowstride, rowstride);
    }
}
