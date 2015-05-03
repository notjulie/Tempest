using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace TempestWpf
{
   class MathBoxLogEntry
   {
      private Dictionary<string, string> attributes = new Dictionary<string, string>();

      public static MathBoxLogEntry FromElement(XmlElement element)
      {
         MathBoxLogEntry result = new MathBoxLogEntry();

         // parse all the child elements
         // parse all the entries
         for (int i = 0; i < element.ChildNodes.Count; ++i)
         {
            XmlElement child = element.ChildNodes[i] as XmlElement;
            if (child == null)
               continue;

            switch (child.Name)
            {
               default:
                  result.attributes[child.Name] = child.GetAttribute("value");
                  break;
            }
         }

         return result;
      }
   }
}
