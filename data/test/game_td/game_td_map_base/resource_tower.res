<?xml version="1.0" ?>
<guiex>
	<property name="TowerBase" type="CGUIImageDefine">
		<property  name="path" type="CGUIString" value="image/tower/TowerBase.png" />
		<property name="uv" type="CGUIRect" value="0,0,78,48" />
		<property name="size" type="CGUISize" value="78,48" />
	</property>

  <!-- animation -->
  <property name="TowerBase" type="CGUIAnimationDefine">
    <property name="size" type="CGUISize" value="78,48"/>
    <property name="interval" type="real" value="0.05"/>
    <property name="loop" type="bool" value="true"/>
    <property name="images" type="array" >
      <property name="image" type="CGUIImage" value="TowerBase" />
    </property>
  </property>
</guiex>
