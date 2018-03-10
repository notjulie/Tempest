//
//  TempestView.swift
//  iTempest
//
//  Created by Randy Rasmussen on 11/15/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

import Foundation
import UIKit
import MetalKit

class TempestView : MTKView {
   private var commandQueue: MTLCommandQueue?
   private var depthStencilState: MTLDepthStencilState?
   private var tempest : cTempest = 0
   private var tempestRenderer : TempestViewRenderer?

   init(tempest : cTempest) {
      // call the super
      super.init(frame: CGRect(), device: MTLCreateSystemDefaultDevice())
      
      // save parameters
      self.tempest = tempest
      
      // View
      clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1)
      colorPixelFormat = .bgra8Unorm
      depthStencilPixelFormat = .depth32Float
      
      // Command queue
      commandQueue = device!.makeCommandQueue()      
      
      // Depth stencil
      let depthSencilDescriptor = MTLDepthStencilDescriptor()
      depthSencilDescriptor.depthCompareFunction = .less
      depthSencilDescriptor.isDepthWriteEnabled = true
      depthStencilState = device!.makeDepthStencilState(descriptor: depthSencilDescriptor)
      
      // now that we're initialized we can create the renderer
      tempestRenderer = TempestViewRenderer(view: self, tempest: tempest)
   }

   required init(coder: NSCoder) {
      super.init(coder: coder)
   }
   
   override func draw(_ dirtyRect: CGRect) {
      // create our parallel render encoder
      let commandBuffer = commandQueue!.makeCommandBuffer()
      let renderPassDescriptor = currentRenderPassDescriptor!
      let parallelRenderEncoder = commandBuffer.makeParallelRenderCommandEncoder(descriptor: renderPassDescriptor)
      
      // create our render encoder for tempest and render it
      let renderEncoder = parallelRenderEncoder.makeRenderCommandEncoder()
      renderEncoder.setFrontFacing(.counterClockwise)
      renderEncoder.setDepthStencilState(depthStencilState)
      
      // render tempest
      tempestRenderer!.render(renderEncoder: renderEncoder)
      renderEncoder.endEncoding()

      // end encoding on our parallel renderer
      parallelRenderEncoder.endEncoding()
      
      let drawable = currentDrawable!
      commandBuffer.present(drawable)
      commandBuffer.commit()
   }
   
}

