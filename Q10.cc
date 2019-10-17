#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/csma-module.h"


using namespace ns3;
NS_LOG_COMPONENT_DEFINE("Second script example");

int main(int argc, char *argv[])
{

uint32_t nCsma=3;


LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);
LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);


NodeContainer p2pNodes;
p2pNodes.Create(3);


NodeContainer csmaNodes;
csmaNodes.Add(p2pNodes.Get(2));
csmaNodes.Create(nCsma);


PointToPointHelper pointToPoint;
pointToPoint.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
pointToPoint.SetChannelAttribute("Delay",StringValue("2ms"));

PointToPointHelper pointToPoint1;
pointToPoint1.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
pointToPoint1.SetChannelAttribute("Delay",StringValue("2ms"));



CsmaHelper csma;
csma.SetChannelAttribute("DataRate",StringValue("10Mbps"));
csma.SetChannelAttribute("Delay",TimeValue(NanoSeconds(6560)));



NetDeviceContainer p2pDevices;
p2pDevices=pointToPoint.Install(p2pNodes.Get(0),p2pNodes.Get(1));
std::cout<<"installed no and n1"<<std::endl;
NetDeviceContainer p2pDevices1;
p2pDevices1=pointToPoint1.Install(p2pNodes.Get(1),p2pNodes.Get(2));
std::cout<<"installed n1 and n2"<<std::endl;

NetDeviceContainer csmaDevices;
csmaDevices=csma.Install(csmaNodes);


InternetStackHelper stack;
stack.Install(p2pNodes);
stack.Install(csmaNodes.Get(1));
stack.Install(csmaNodes.Get(2));
stack.Install(csmaNodes.Get(3));




Ipv4AddressHelper address;
address.SetBase("10.1.1.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces;
p2pInterfaces=address.Assign(p2pDevices);


address.SetBase("10.1.2.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces1;
p2pInterfaces1=address.Assign(p2pDevices1);



Ipv4AddressHelper address3;
address3.SetBase("20.1.2.0", "255.255.255.0");
Ipv4InterfaceContainer csmaInterfaces;
csmaInterfaces=address3.Assign(csmaDevices);



UdpEchoServerHelper echoServer(9);
ApplicationContainer serverApps = echoServer.Install(csmaNodes.Get(1));
serverApps.Start(Seconds(1.0));
serverApps.Stop(Seconds(10.0));




UdpEchoClientHelper echoClient(csmaInterfaces.GetAddress(1),9);
echoClient.SetAttribute("MaxPackets", UintegerValue(1));
echoClient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
echoClient.SetAttribute("PacketSize", UintegerValue(1024));

ApplicationContainer clientApps=echoClient.Install(p2pNodes.Get(0));
clientApps.Start(Seconds(2.0));
clientApps.Stop(Seconds(10.0));

UdpEchoServerHelper echoServer1(10);
ApplicationContainer serverApps1 = echoServer1.Install(csmaNodes.Get(1));
serverApps1.Start(Seconds(2.0));
serverApps1.Stop(Seconds(10.0));




UdpEchoClientHelper echoClient1(csmaInterfaces.GetAddress(1),10);
echoClient1.SetAttribute("MaxPackets", UintegerValue(1));
echoClient1.SetAttribute("Interval",TimeValue(Seconds(1.0)));
echoClient1.SetAttribute("PacketSize", UintegerValue(1024));

ApplicationContainer clientApps1=echoClient1.Install(p2pNodes.Get(1));
clientApps1.Start(Seconds(3.0));
clientApps1.Stop(Seconds(10.0));




Ipv4GlobalRoutingHelper::PopulateRoutingTables();
//pointToPoint.EnablePcapAll("second");
//csma.EnablePcap("second",csmaDevices.Get(1),true);
//pointToPoint1.EnablePcapAll("second");
Simulator::Run();
Simulator::Destroy();
return 0;
}
