#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Lab3Q1Example");
int main(int argc, char*argv[])
{
   bool verbose = true;
   uint32_t ncsma = 5;
   CommandLine cmd (__FILE__);
   cmd.AddValue("ncsma","No. of extranodes");
   cmd.AddValue("verbose","The echoApplications to tag of terms",verbose);
   cmd.Parse(argc,argv);
   ncsma=ncsma==0?1:ncsma;
   NodeContainer p2pNodes;
   p2pNodes.Create(2);
   NodeContainer csma;
   csma.Add(p2pNodes.Get(1));
   csma.Create(ncsma);
   PointToPointHelper p2p1;
   p2p1.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
   p2p1.SetChannelAttribute("Delay",StringValue("2ms"));
   NetDeviceContainer p2pd1;
   p2pd1 = p2p1.Install(p2pNodes.Get(0),p2pNodes.Get(1));
   CsmaHelper csmaHelp;
   csmaHelp.SetChannelAttribute("DataRate",StringValue("100Mbps"));
   csmaHelp.SetChannelAttribute("Delay",TimeValue(NanoSeconds(6560)));
   NetDeviceContainer csmad;
   csmad = csmaHelp.Install(csma);
   InternetStackHelper stack;
   stack.Install(p2pNodes.Get(0));
   stack.Install(csma);
   Ipv4AddressHelper address;
   address.SetBase("10.1.1.0","255.255.255.0");
   Ipv4InterfaceContainer csmaint;
   csmaint = address.Assign(csmad);
   UdpEchoServerHelper echoServer(9);
   ApplicationContainer serverApps = echoServer.Install(csma.Get(ncsma));
   serverApps.Start(Seconds(1.0));
   serverApps.Stop(Seconds(10.0));
   UdpEchoClientHelper echoClient(csmaint.GetAddress(ncsma),9);
   echoClient.SetAttribute("Max Packets",IntegerValue(1));
   echoClient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
   echoClient.SetAttribute("PacketS Size",UintegerValue(1024));
   ApplicationContainer clientApps = echoClient.Install(p2pNodes.Get(0));
   clientApps.Start(Seconds(1.0));
   clientApps.Stop(Seconds(10.0));
   Ipv4GlobalRoutingHelper::PopulateRoutingTables();
   AsciiTraceHelper ascii;
   p2p1.EnableAsciiAll(ascii.CreateFileStream("Lab3Q1.tr"));
   Simulator::Run();
   Simulator::Destroy();
 }
   
   
