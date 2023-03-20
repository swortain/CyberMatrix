'''
Date: 2023-03-16 15:28:59
LastEditors: swortain jianshan_54@126.com
LastEditTime: 2023-03-16 16:42:49
FilePath: \221025_led_matrix\build_script.py
Description: 
'''
Import("env")
env.Append(LIBPATH=[env['PROJECT_DIR']+'/lib/displayVFX'])
Library('displayVFX', ['displayVFX.h'])
env.Append(LIBS=['displayVFX'])