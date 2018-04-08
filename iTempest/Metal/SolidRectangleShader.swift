//
//  SolidRectangleShader.swift
//  iTempest
//
//  Created by Randy Rasmussen on 4/8/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import MetalKit

class SolidRectangleShader : RectangleShader {
   init(view:MTKView) {
      super.init(view:view, shaderName:"solidRectangleFragmentFunction")
      
      setColor(r:0, g:0, b:0, a:1)
   }
   
   public func setColor(r:Float, g:Float, b:Float, a:Float) {
      let params :  SolidRectangleRenderParameters =
         SolidRectangleRenderParameters(
            r:r,
            g:g,
            b:b,
            a:a
            )
      addFragmentBuffer(index:Int(SOLID_RECTANGLE_RENDER_PARAMETERS_BUFFER), o:params)
   }
}
