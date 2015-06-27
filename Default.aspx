<%@ Page Async="true" AutoEventWireup="true" Inherits="Advent" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>Adventure game</title>
    <style type="text/css">
            .MultiLine {
                resize:none;
            }
    </style>
</head>
<body>
    <form id="Main" runat="server">
        <asp:TextBox ID="Log" runat="server" ReadOnly="true" Width="100%" Height="500px" TextMode="MultiLine" CssClass="MultiLine"></asp:TextBox>
        <asp:TextBox ID="Command" runat="server" Width="80%"></asp:TextBox>
        <asp:Button id="Button" runat="server" Text="Отправить" OnClick="Send" Width="18%"></asp:Button>
    </form>
</body>
</html>
