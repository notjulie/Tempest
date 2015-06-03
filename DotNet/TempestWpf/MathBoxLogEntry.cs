using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace TempestWpf
{
   /// <summary>
   /// Represents an entry in the MathBox log
   /// </summary>
   public class MathBoxLogEntry
   {
      private Dictionary<string, string> attributes = new Dictionary<string, string>();
      private List<ALULogEntry> alu = new List<ALULogEntry>();

      /// <summary>
      /// Gets the value of a named attribute
      /// </summary>
      /// <param name="name">the name</param>
      /// <returns>the value</returns>
      public string GetAttribute(string name)
      {
         if (attributes.ContainsKey(name))
            return attributes[name];
         else
            return string.Empty;
      }

      /// <summary>
      /// Gets the named ALU's log entry
      /// </summary>
      /// <param name="name">the name</param>
      /// <returns>the entry</returns>
      public ALULogEntry GetALU(string name)
      {
         foreach (ALULogEntry logEntry in alu)
            if (logEntry.Name == name)
               return logEntry;

         return new ALULogEntry();
      }

      /// <summary>
      /// Constructs an instance from the given XML element
      /// </summary>
      /// <param name="element">the XML element</param>
      /// <returns>the resulting instance</returns>
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
               case "ALU":
                  result.alu.Add(ALULogEntry.FromXML(child));
                  break;

               default:
                  result.attributes[child.Name] = child.GetAttribute("value");
                  break;
            }
         }

         return result;
      }
   }
}
