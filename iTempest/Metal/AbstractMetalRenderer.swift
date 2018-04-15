//
//  AbstractMetalRenderer.swift
//  iTempest
//
//  Created by Randy Rasmussen on 4/1/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import MetalKit

protocol AbstractMetalRenderer {
   func render(renderEncoder : MTLRenderCommandEncoder)
}
