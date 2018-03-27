//
//  PlayerButtonRenderer.swift
//  iTempest
//
//  Created by Randy Rasmussen on 3/25/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import MetalKit

class PlayerButtonRenderer : RectangleShader {
   init(view:MTKView) {
      // call the super
      super.init(view: view, shaderName:"playerButtonShader")
   }
}
