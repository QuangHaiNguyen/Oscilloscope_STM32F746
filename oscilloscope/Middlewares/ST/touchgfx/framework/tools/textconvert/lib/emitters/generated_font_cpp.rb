# Copyright (c) 2018(-2023) STMicroelectronics.
# All rights reserved.
#
# This file is part of the TouchGFX 4.22.1 distribution.
#
# This software is licensed under terms that can be found in the LICENSE file in
# the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
#
###############################################################################/
class GeneratedFontCpp < Template
  def input_path
    File.join(root_dir,'Templates','GeneratedFont.cpp.temp')
  end
  def output_path
    '/src/GeneratedFont.cpp'
  end
  def output_filename
    File.join(@output_directory, output_path)
  end
  def run
    if !File::exists?(output_filename)
      #generate GeneratedFont.cpp
      super
    end
  end
end
