#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/application-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("CSMA MULTICAST");
int main(int argc, char* argv[])
{
 Config :: SetDefault("ns3" :: OnoffApplication :: PacketSize, uintegerValue(137));
 Config :: SetDefault("ns3" :: OnoffApplication :: DataRate, StringValue("14Kbps"));
 uint32_t nspoles = 8;
 CommandLine cmd (__FILE__);
 cmd.AddValue("nspoles","number of nodes to place in the star",nspoles);
 cmd.Parse(argc,argv);
 PointToPointHelper p2p;
 p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
 p2p.SetChannelAttribute("Delay", StringValue("2ms"));
 PointToPointStarHelper star(nspoles,p2p);
 InternetStackHelper intersect;
 star.InstallStack(intersect);
 star.AssignIpv4Addresses (Ipv4AddressHelper("10.1.1.0","255.255.255.0");
 
 uint16_t port = 5000;
 Address hubLocalAddress(InetSocketAddress(Ipv4Address::GetAny(),port));
 PacketStackHelper packetStackHelper("ns3:TcpSocketFactory",hubLocalAddress);
 ApplicationContainer hubApp = packetStackHelper.Install(star.getHub());
 hubApp.Start(Seconds(1.0));
 hubApp.Stop(Seconds(10.0));
 OnoffHelper onf("ns3::TcpSocketFactory",Address());
 onf.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]");
 onf.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]");
 ApplicationContainer SpokeApps;
 for(uint32_t i =0; i<star.spokeCount();++i)
 {
   AddressValue remoteAddress(InetSocketAddress(star.GetHubIpv4Address(i).port));
   onf.SetAttribute("Remote",remoteAddress);
   spokeApps.Add(onf.Install(star.GetSpokesNode(i)));
 }
 
 spokeApps.Start(Seconds(1.0));
 spokeApps.Stop(Seconds(10.0));
 Ipv4GlobalRoutingHelper :: PopulateRoutingTables();
 AsciiTraceHelper ascii;
 p2p.EnableAsciiAll(ascii.CreateFileStream("L4Q2.tr"));
 AnimationInterface anim("L4Q2.xml");
 Simulator::Run();
 Simulator::Destroy();
 }
