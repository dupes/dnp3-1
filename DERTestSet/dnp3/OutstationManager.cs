using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DNP3.Adapter;
using DNP3.Interface;

namespace DERTestSet.dnp3
{
    public class OutstationManager
    {
        IDNP3Manager m_manager = DNP3ManagerFactory.CreateManager(Environment.ProcessorCount);

        Dictionary<int, DEROutstation> m_outstations = new Dictionary<int, DEROutstation>();
        Dictionary<string, int> m_outstationNameToIndex = new Dictionary<string, int>();

        /*********************************************************************************************/

        public OutstationManager()
        {
        }

        /*********************************************************************************************/

        public void addOutstation(string name, string ipAddress, ushort port)
        {
            IChannel channel = m_manager.AddTCPClient(name, LogFilters.INFO, new TimeSpan(0, 0, 5), new TimeSpan(0, 0, 10), ipAddress, port);

            OutstationStackConfig config = new OutstationStackConfig();

            DEROutstation derOutstation = new DEROutstation();

            IOutstation outstation = channel.AddOutstation(name, derOutstation, derOutstation, config);

            derOutstation.setOutstation(outstation);

            int index = m_outstations.Count;

            m_outstations.Add(index, derOutstation);
            m_outstationNameToIndex.Add(name, index);

            outstation.Enable();
        }
    }
}
